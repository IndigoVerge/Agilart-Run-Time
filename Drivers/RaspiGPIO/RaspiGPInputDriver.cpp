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

#include "Consts.h"
#include "RaspiGPInputDriver.h"
#include "bcm2835.h"
#include "Logging/EventLogger.h"

#define DriverName  "RaspberryPi GPIO Input Driver";

using namespace std;

RaspiGPInputDriver::RaspiGPInputDriver() : BaseDriver()
{
	Command initCommand( INIT_CMD );
	initCommand.AddParameter(
			new CommandParameter( GPIO_NUMBER_PARAM, NULL, "int" ) );

	this->commands->push_back( initCommand );
	this->supportedComands->push_back( initCommand );

	Event changeValueEvent( CHVAL_EVENT );
	changeValueEvent.AddParameter(EventParameter( GPIOKEY_CODE_PARAM, 0, "int" ));
	changeValueEvent.AddParameter(EventParameter( GPIOKEY_TYPE_PARAM, 0, "int" ));
	changeValueEvent.AddParameter(EventParameter( GPIOKEY_VALUE_PARAM, 0, "int" ));

	this->suportedEvents->push_back( changeValueEvent );

	Command stopCommand( STOP_CMD );
	this->supportedComands->push_back( stopCommand );
}

RaspiGPInputDriver::~RaspiGPInputDriver()
{
	// implemented in the BaseDriver
}

const char* RaspiGPInputDriver::Type()
{
	return "GPIOKeys";
}

const char* RaspiGPInputDriver::Version()
{
	return "0.0.1";
}

const char* RaspiGPInputDriver::Name()
{
	return DriverName;
}

const char* RaspiGPInputDriver::Description()
{
	return "Raspberry Pi GPIO Input Driver";
}

const char* RaspiGPInputDriver::Publisher()
{
	return "CompletIT Ltd.";
}

void RaspiGPInputDriver::ExecuteCommand( Command command )
{
	string logMessage = "RaspiGPInputDriver - Execute Command "	+ command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	if ( !strcmp( command.Name().c_str(), INIT_CMD ) )
	{
		CommandParameter* portnumberParam = command.GetParameter(
				GPIO_NUMBER_PARAM );

		this->keyCode = *((int*) ( portnumberParam->Value ));
		this->shouldStop = false;

		this->OpenPort();
		this->UpdateInitialState();
		this->StartReadingKeyThread();

	}
	else if( !strcmp( command.Name().c_str(), STOP_CMD ) )
	{
		this->StopReadingKeyThread();
	}
}

Command* RaspiGPInputDriver::GetCommand( const char* name )
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

list<Command> RaspiGPInputDriver::Commands()
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

BaseDriver* RaspiGPInputDriver::Clone()
{
	BaseDriver* driver = new RaspiGPInputDriver();
	return driver;
}

int RaspiGPInputDriver::OpenPort()
{
	return bcm2835_init();
}

void RaspiGPInputDriver::ClosePort()
{
	//close( this->fdKey );
}

void RaspiGPInputDriver::SetCurrentValue( int newValue )
{
	list<EventParameter>* parameters = new list<EventParameter>();
	parameters->push_back( EventParameter( GPIOKEY_CODE_PARAM, newValue, "int" ) );
	parameters->push_back( EventParameter( GPIOKEY_TYPE_PARAM, newValue, "int" ) );
	parameters->push_back( EventParameter( GPIOKEY_VALUE_PARAM, newValue, "int" ) );

	Event event( CHVAL_EVENT, parameters );

	//Log
	EventLogger::Instance()->WriteVerbose( "RaspiGPInputDriver - StatusChangedEvent Key Code: %d Value: %d",
			newValue, newValue );

	this->StatusChangedEvent( this, event );
}

int RaspiGPInputDriver::ReadValue()
{
	int gpioValue = 0;
	while ( true )
	{
		if( this->shouldStop == true )
			break;

		gpioValue = bcm2835_gpio_lev(this->keyCode);

		if ( gpioValue != this->currentValue )
			break;
	}

	return gpioValue;
}

void* RaspiReadKeyThreadRun( void* param )
{
	RaspiGPInputDriver* driver = (RaspiGPInputDriver*) param;

	int newValues;
	bool shouldStop = false;

	while ( true )
	{
		newValues = driver->ReadValue();

		pthread_mutex_lock( &driver->commonResourcesMutex );
		shouldStop = driver->shouldStop;
		pthread_mutex_unlock( &driver->commonResourcesMutex );

		if(shouldStop == true)
			break;

		driver->SetCurrentValue( newValues );
	}

	pthread_exit( (void *) 0 );
}

void RaspiGPInputDriver::StartReadingKeyThread()
{
	//cancelThreadMutex
	int returnStatus;
	returnStatus = pthread_mutex_init( &this->commonResourcesMutex, NULL );

	if ( returnStatus != 0 )
	{
		EventLogger::Instance()->WriteError( ((string)("Error creating GPIOKey Driver Common Resources Mutex!")).c_str() );
		return;
	}

	returnStatus = pthread_create( &this->readKeyThread, NULL,
			RaspiReadKeyThreadRun, this );

	if ( returnStatus != 0 )
	{
		EventLogger::Instance()->WriteError( ((string)("Error creating GPIOKey driver read key thread!")).c_str() );
		return;
	}
}

void RaspiGPInputDriver::StopReadingKeyThread()
{
	pthread_mutex_lock( &this->commonResourcesMutex );
	this->shouldStop = true;
	pthread_mutex_unlock( &this->commonResourcesMutex );

	this->ClosePort();

	//pthread_join( this->readKeyThread, NULL );

	string logMessage = "RaspiGPInputDriver - Stop thread succeeded. ";
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );
}

void RaspiGPInputDriver::UpdateInitialState()
{
	// input_event initialValue = this->ReadValue();
	// this->SetCurrentValue( initialValue );
}
