/*******************************************************************************
 * Agilart Run-time runs on microcontrollers or other compatible hardware and
 * allows you to create, execute and monitor programs remotely via simple web API.
 * To make programs, Agilart Visual Designer can be used at www.agilart.com.
 *
 * Copyright (C) 2012  CompletIT (www.agilart.com), Ivan Dragoev (idragoev@completit.com),
 * Bobi Rakova (brakova@completit.com)
 *
 * This file is part of Agilart Run-time.
 *
 * Agilart Run-time is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Agilart Run-time is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************************/

#include "TemperatureDriver.h"
#include "Consts.h"
#include "Logging/EventLogger.h"

#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define TMP_DEVICENAME "Device Name"
#define TMP_DEVICEADDRESS "Device Address"
#define TMP_REFRESH "Refresh Time"
#define TMP_VALUE "Value"
#define TMP_CHANGED "Temperature changed"
#define TMP_DEVNAME "Temperature Sensor Driver"
#define TMP_DEVTYPE "TmpDevice"
#define TMP_VERSION "0.0.1"

TemperatureDriver::TemperatureDriver() : BaseDriver()
{
	Command initCommand( INIT_CMD );
	initCommand.AddParameter( new CommandParameter( TMP_DEVICENAME, new string("/dev/i2c-0"), "string" ) );
	initCommand.AddParameter( new CommandParameter( TMP_DEVICEADDRESS, new string("0x48"), "string" ) );
	initCommand.AddParameter( new CommandParameter( TMP_REFRESH, new int(1000), "int" ) );

	this->commands->push_back( initCommand );
	this->supportedComands->push_back( initCommand );

	Event changeValueEvent( TMP_CHANGED );
	changeValueEvent.AddParameter(EventParameter( TMP_VALUE, 0, "float" ));
	this->suportedEvents->push_back( changeValueEvent );

	Command stopCommand( STOP_CMD );
	this->supportedComands->push_back( stopCommand );
}

TemperatureDriver::~TemperatureDriver()
{
	// implemented in the BaseDriver
}

const char* TemperatureDriver::Type()
{
	return TMP_DEVTYPE;
}

const char* TemperatureDriver::Version()
{
	return TMP_VERSION;
}

const char* TemperatureDriver::Name()
{
	return TMP_DEVNAME;
}

const char* TemperatureDriver::Description()
{
	return TMP_DEVNAME;
}

const char* TemperatureDriver::Publisher()
{
	return "CompletIT Ltd.";
}

void TemperatureDriver::ExecuteCommand( Command command )
{
	EventLogger::Instance()->WriteVerbose("Temperature Driver - Execute Command %s", command.Name().c_str());

	if ( !strcmp( command.Name().c_str(), INIT_CMD ) )
	{
		CommandParameter* nameParam = command.GetParameter( TMP_DEVICENAME );
		CommandParameter* addressParam = command.GetParameter( TMP_DEVICEADDRESS );
		CommandParameter* refreshParam = command.GetParameter( TMP_REFRESH );

		this->devName = "/dev/" + *((string*) ( nameParam->Value ));
		this->address = *((string*) ( addressParam->Value ));
		this->refreshOn = *((int*) ( refreshParam->Value ));
		this->shouldStop = false;

		if( this->OpenPort() )
			this->StartReadingThread();
	}
	else if( !strcmp( command.Name().c_str(), STOP_CMD ) )
	{
		this->StopReadingThread();
	}
}

Command* TemperatureDriver::GetCommand( const char* name )
{
	list<Command>::iterator it;
	for ( it = this->commands->begin(); it != this->commands->end(); it++ )
	{
		if ( !strcmp( ( *it ).Name().c_str(), name ) )
		{
			return ( *it ).Clone();
		}
	}

	return NULL;
}

list<Command> TemperatureDriver::Commands()
{
	list<Command> commandsSet;

	list<Command>::iterator it;

	for ( it = this->commands->begin(); it != this->commands->end(); it++ )
	{
		Command* c = ( *it ).Clone();
		commandsSet.push_back( *c );
	}

	return commandsSet;
}

BaseDriver* TemperatureDriver::Clone()
{
	BaseDriver* driver = new TemperatureDriver();
	return driver;
}

int TemperatureDriver::OpenPort()
{
	cout<<"Opening: "<<this->devName<<" at: "<<this->address<<endl;
	if ((this->fd = open(this->devName.c_str(), O_RDWR)) < 0) {                               // Open port for reading and writing
			printf("Failed to open i2c port\n");
			return 0;
	}

	int intAddress;
	sscanf(this->address.c_str(), "%x", &intAddress);
	printf("%d, %u\n", intAddress, intAddress);

	if (ioctl(this->fd, I2C_SLAVE, intAddress) < 0) {  // Set the port options and set the address of the device we wish to speak to
			printf("Unable to get bus access to talk to slave\n");
			return 0;
	}

	return this->fd;
}

void TemperatureDriver::ClosePort()
{
	close( this->fd );
}

void TemperatureDriver::SetCurrentValue( float newValue )
{
	this->currentValue = newValue;
	list<EventParameter>* parameters = new list<EventParameter>();
	parameters->push_back( EventParameter( TMP_VALUE, newValue, "float" ) );
	Event event( TMP_CHANGED, parameters );

	//Log
	EventLogger::Instance()->WriteVerbose( "TemperatureDriver - StatusChangedEvent Value: %f", newValue );
	this->StatusChangedEvent( this, event );
}

float TemperatureDriver::ReadValue()
{
	unsigned char buf[10];
	unsigned char msb;
	unsigned char lsb;

	unsigned int temp = 0;
	float tempInCelsius;

	if (read(this->fd, buf, 1) != 1) {
			printf("Unable to read from slave\n");
			exit(1);
	}
	else {
			msb = buf[0];
			printf("0x%02X ", msb);

			if (read(this->fd, buf, 1) != 1) {
					printf("Unable to read from slave\n");
					EventLogger::Instance()->WriteError("Unable to read from slave FD: %d\n", this->fd);
			} else {
					lsb = buf[0];
					temp = (msb<<8) | lsb;
					temp >>= 4;

					tempInCelsius = temp * 0.0625;
#ifdef DEBUG
					printf("Temperature: %f \n", tempInCelsius);
#endif
			}
	}

	return tempInCelsius;
}

void* ReadTempertureThreadRun( void* param )
{
	TemperatureDriver* driver = (TemperatureDriver*) param;

	clock_t endwait;
	float newValue;
	bool shouldStop = false;
	float refreshTimeInSecs = driver->refreshOn / 1000;

	while ( true ) {
		newValue = driver->ReadValue();

		pthread_mutex_lock( &driver->commonResourcesMutex );
		shouldStop = driver->shouldStop;
		pthread_mutex_unlock( &driver->commonResourcesMutex );

		if(shouldStop == true)
			break;

		if( driver->currentValue != newValue )
			driver->SetCurrentValue( newValue );

		endwait = clock() + refreshTimeInSecs * CLOCKS_PER_SEC;

		while (clock() < endwait) {
			// wait
		}
	}

	driver->ClosePort();
	pthread_exit( (void *) 0 );
}

void TemperatureDriver::StartReadingThread()
{
	int returnStatus = pthread_mutex_init( &this->commonResourcesMutex, NULL );

	if ( returnStatus != 0 )
	{
		EventLogger::Instance()->WriteError( ((string)("Error creating Temperature Driver Common Resources Mutex!")).c_str() );
		return;
	}

	returnStatus = pthread_create( &this->readKeyThread, NULL,
			ReadTempertureThreadRun, this );

	if ( returnStatus != 0 )
	{
		EventLogger::Instance()->WriteError( ((string)("Error creating Temperature driver read key thread!")).c_str() );
		return;
	}
}

void TemperatureDriver::StopReadingThread()
{
	pthread_mutex_lock( &this->commonResourcesMutex );
	this->shouldStop = true;
	pthread_mutex_unlock( &this->commonResourcesMutex );

	string logMessage = "TemperatureDriver - Stop thread succeeded. ";
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );
}
