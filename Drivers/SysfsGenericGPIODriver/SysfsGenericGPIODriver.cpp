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

#include <iostream>
#include "SysfsGenericGPIODriver.h"
#include "Logging/EventLogger.h"

using namespace std;

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64

SysfsGenericGPIODriver::SysfsGenericGPIODriver() : BaseDriver()
{
	Command initCommand( INIT_CMD );
	initCommand.AddParameter( new CommandParameter( PORTNUMBER_KEY, NULL, "int" ) );
	initCommand.AddParameter( new CommandParameter( DIRECTION_KEY, NULL, "char*" ) );

	Command setValueCommand( SETVAL_CMD );
	setValueCommand.AddParameter( new CommandParameter( VALUE_KEY, NULL, "int" ) );

	this->commands->push_back( initCommand );
	this->commands->push_back( setValueCommand );

	this->supportedComands->push_back( initCommand );
	this->supportedComands->push_back( setValueCommand );

	Command stopCommand( STOP_CMD );
	this->supportedComands->push_back( stopCommand );
}

SysfsGenericGPIODriver::~SysfsGenericGPIODriver()
{
}

const char* SysfsGenericGPIODriver::Type()
{
	return "GPIO";
}

const char* SysfsGenericGPIODriver::Version()
{
	return "0.0.1";
}

const char* SysfsGenericGPIODriver::Name()
{
	return "SysfsGPIO";
}

const char* SysfsGenericGPIODriver::Description()
{
	return "Sysfs Generic GPIO driver";
}

const char* SysfsGenericGPIODriver::Publisher()
{
	return "";
}

void* SysfsThreadRun( void* param )
{
	SysfsGenericGPIODriver* driver = (SysfsGenericGPIODriver*) param;

	int newStatus;

	while ( true )
	{
		pthread_mutex_lock( &driver->commonResourcesMutex );
		if(driver->shouldStop == true)
				break;
		pthread_mutex_unlock( &driver->commonResourcesMutex );

		newStatus = driver->GetGpioValue();

		if ( driver->GetStatus() != newStatus )
		{
			driver->SetStatus( newStatus );
			driver->SignalPortstatusChanged();
		}

		for ( int j = 0; j < 1000000; j++ )
			; // Delay
	}

	pthread_exit( (void *) 0 );
}

int SysfsGenericGPIODriver::GetStatus()
{
	return this->status;
}

void SysfsGenericGPIODriver::SetStatus( int value )
{
	this->status = value;
}

int SysfsGenericGPIODriver::GetPortNumber()
{
	return this->portNumber;
}

int SysfsGenericGPIODriver::GetGpioValue()
{
	int newStatus;
	int statusResult = this->gpio_get_value( this->portNumber, &newStatus );

	if ( statusResult != 0 )
	{
		cout << "Error in Sysfs GPIO get value\n";
		throw "Error in Sysfs GPIO get value";
	}

	return statusResult;
}

void SysfsGenericGPIODriver::SignalPortstatusChanged()
{
	list<EventParameter>* parameters = new list<EventParameter>();

	int portNumber = this->GetPortNumber();
	int curStaus = this->GetStatus();

	char numstr[21];
	sprintf( numstr, "%d", portNumber );
	string portNumberString = numstr;
	sprintf( numstr, "%d", curStaus );
	string curStausString = numstr;

	string logMessage = "SysfsGenericGPIODriver - SignalPortstatusChanged Port: " + portNumberString +
			" Status: " + curStausString;
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	parameters->push_back( EventParameter( PORTNUMBER_KEY, portNumber, "int" ) );
	parameters->push_back( EventParameter( VALUE_KEY, curStaus, "int" ) );

	Event event( CHVAL_EVENT, parameters );

	this->StatusChangedEvent( this, event );
}

void SysfsGenericGPIODriver::StartPoolingForStatus()
{
	int returnStatus;

	returnStatus = pthread_mutex_init( &this->commonResourcesMutex, NULL );

	if ( returnStatus != 0 )
	{
		cout << "Error creating SysfsGenericGPIODriver driver Common Resources Mutex!";
		return;
	}

	returnStatus = pthread_create( &this->refreshStatusThread, NULL,
			SysfsThreadRun, this );

	if ( returnStatus != 0 )
	{
		cout << "Error creating SysfsGenericGPIODriver status update thread!";
		return;
	}
}

void SysfsGenericGPIODriver::StopPoolingForStatus()
{
	pthread_mutex_lock( &this->commonResourcesMutex );
	this->shouldStop = true;
	pthread_mutex_unlock( &this->commonResourcesMutex );

	this->ClosePort( this->portNumber );

	//pthread_join( this->refreshStatusThread, NULL );

	string logMessage = "SysfsGenericGPIODriver - Stop thread succeeded. ";
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );
}

void SysfsGenericGPIODriver::ExecuteCommand( Command command )
{
	string logMessage = "SysfsGenericGPIODriver - Execute Command " + command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	if ( !strcmp( command.Name().c_str(), INIT_CMD ) )
	{
		string direction;
		int outFlag = 0;

		CommandParameter* portnumberParam =	command.GetParameter( PORTNUMBER_KEY );
		CommandParameter* directionParam = command.GetParameter( DIRECTION_KEY );

		if ( this->portNumber != 0 )
			this->ClosePort( this->portNumber );

		this->portNumber = *(int*) ( portnumberParam->Value );
		direction = *((string*)( directionParam->Value ));

		if ( direction == "OUT" )
			outFlag = 1;

		this->shouldStop = false;
		this->OpenPort( this->portNumber, outFlag );
		// Trigger a 1-time event to notify the device of its current state.
		this->SignalPortstatusChanged();
		this->StartPoolingForStatus();
	}
	else if ( !strcmp( command.Name().c_str(), SETVAL_CMD ) )
	{
		CommandParameter* valueParam = command.GetParameter( VALUE_KEY );
		int value = *(int*) ( valueParam->Value );

		//cout<<"SYSFS - Set Value - port "<<this->portNumber<<" value - "<<value<<endl;
		char msg[1024];
		sprintf(msg, "SYSFS - Set Value - port %d  value - %d ", this->portNumber, value);
		string logMessage =msg;
		EventLogger::Instance()->WriteInformation( logMessage.c_str() );

		if ( ( value == 0 || value == 1 ) && this->portNumber != 0 )
			this->SetValue( this->portNumber, value );
	}
	else if ( !strcmp( command.Name().c_str(), STOP_CMD ) )
	{
		this->StopPoolingForStatus();
	}
}

list<Command> SysfsGenericGPIODriver::Commands()
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

BaseDriver* SysfsGenericGPIODriver::Clone()
{
	BaseDriver* clone = new SysfsGenericGPIODriver();
	return clone;
}

Command* SysfsGenericGPIODriver::GetCommand( const char* name )
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

int SysfsGenericGPIODriver::OpenPort( int portnumber, int isOutFlag )
{
	int exportResult = this->gpio_export( portnumber );

	if ( exportResult < 0 )
		throw "Cannot export gpio";

	exportResult = this->gpio_set_dir( portnumber, isOutFlag );

	if ( exportResult < 0 )
		throw "Cannot set gpio direction";

	int fd, len;
	char buf[MAX_BUF];

	len = snprintf( buf, sizeof( buf ), SYSFS_GPIO_DIR "/gpio%d/value",
			portnumber );

	fd = open( buf, O_RDWR );
	if ( fd < 0 )
		throw "Cannot access value";

	this->gpioFD = fd;

	return 0;
}

int SysfsGenericGPIODriver::ClosePort( int portnumber )
{
	this->gpio_unexport( portnumber );

	return 0;
}

void SysfsGenericGPIODriver::SetValue( int portnumber, int value )
{
	int setValueResult = this->gpio_set_value( portnumber, value );

	if ( setValueResult != 0 )
		perror( "Set GPIO Value fails to succeed." );
}

int SysfsGenericGPIODriver::GetValue( int portnumber )
{
	int value;
	int getValueResult = this->gpio_get_value( portnumber, &value );

	if ( getValueResult != 0 )
		perror( "Get GPIO Value fails to succeed." );

	return value;
}

/****************************************************************
 * gpio_export
 ****************************************************************/
int SysfsGenericGPIODriver::gpio_export( unsigned int gpio )
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open( SYSFS_GPIO_DIR "/export", O_WRONLY );
	if ( fd < 0 )
	{
		perror( "gpio/export" );
		return fd;
	}

	len = snprintf( buf, sizeof( buf ), "%d", gpio );
	write( fd, buf, len );
	close( fd );

	return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int SysfsGenericGPIODriver::gpio_unexport( unsigned int gpio )
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open( SYSFS_GPIO_DIR "/unexport", O_WRONLY );
	if ( fd < 0 )
	{
		perror( "gpio/export" );
		return fd;
	}

	len = snprintf( buf, sizeof( buf ), "%d", gpio );
	write( fd, buf, len );
	close( fd );
	return 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int SysfsGenericGPIODriver::gpio_set_dir( unsigned int gpio,
		unsigned int out_flag )
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf( buf, sizeof( buf ), SYSFS_GPIO_DIR "/gpio%d/direction",
			gpio );

	fd = open( buf, O_WRONLY );
	if ( fd < 0 )
	{
		perror( "gpio/direction" );
		return fd;
	}

	if ( out_flag )
		write( fd, "out", 4 );
	else
		write( fd, "in", 3 );

	close( fd );
	return 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int SysfsGenericGPIODriver::gpio_set_value( int gpio, int value )
{
	lseek( this->gpioFD, 0, SEEK_SET );

	if ( this->gpioFD < 0 )
	{
		perror( "gpio/set-value" );
		return this->gpioFD;
	}

	if ( value )
		write( this->gpioFD, "1", 2 );
	else
		write( this->gpioFD, "0", 2 );

	return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int SysfsGenericGPIODriver::gpio_get_value( int gpio, int *value )
{
	char ch;

	lseek( this->gpioFD, 0, SEEK_SET );

	if ( this->gpioFD < 0 )
	{
		perror( "gpio/get-value" );
		return this->gpioFD;
	}

	read( this->gpioFD, &ch, 1 );

	if ( ch != '0' )
	{
		*value = 1;
	}
	else
	{
		*value = 0;
	}

	return 0;
}

/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int SysfsGenericGPIODriver::gpio_set_edge( unsigned int gpio, char *edge )
{
//	int fd, len;
//	char buf[MAX_BUF];
//
//	len = snprintf( buf, sizeof( buf ), SYSFS_GPIO_DIR "/gpio%d/edge", gpio );
//
//	fd = open( buf, O_WRONLY );
//	if ( fd < 0 )
//	{
//		perror( "gpio/set-edge" );
//		return fd;
//	}
//
//	write( fd, edge, strlen( edge ) + 1 );
//	close( fd );
	return 0;
}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int SysfsGenericGPIODriver::gpio_fd_open( unsigned int gpio )
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf( buf, sizeof( buf ), SYSFS_GPIO_DIR "/gpio%d/uevent", gpio );

	fd = open( buf, O_RDONLY | O_NONBLOCK );
	if ( fd < 0 )
	{
		perror( "gpio/fd_open" );
	}
	return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int SysfsGenericGPIODriver::gpio_fd_close( int fd )
{
	return close( fd );
}
