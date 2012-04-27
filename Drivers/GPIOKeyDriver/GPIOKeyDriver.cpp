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

#include <errno.h>
#include <iostream>
#include "Consts.h"
#include "GPIOKeyDriver.h"
#include "Logging/EventLogger.h"

#define DriverName  "GPIO Input Driver";

using namespace std;

GPIOKeyDriver::GPIOKeyDriver() : BaseDriver()
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

GPIOKeyDriver::~GPIOKeyDriver()
{
	// implemented in the BaseDriver
}

const char* GPIOKeyDriver::Type()
{
	return "GPIOKeys";
}

const char* GPIOKeyDriver::Version()
{
	return "0.0.1";
}

const char* GPIOKeyDriver::Name()
{
	return DriverName;
}

const char* GPIOKeyDriver::Description()
{
	return "Driver for handling GPIO based key.";
}

const char* GPIOKeyDriver::Publisher()
{
	return "CompletIT Ltd.";
}

void GPIOKeyDriver::ExecuteCommand( Command command )
{
	string logMessage = "GPIOKeyDriver - Execute Command "	+ command.Name();
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

Command* GPIOKeyDriver::GetCommand( const char* name )
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

list<Command> GPIOKeyDriver::Commands()
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

BaseDriver* GPIOKeyDriver::Clone()
{
	BaseDriver* driver = new GPIOKeyDriver();
	return driver;
}

int GPIOKeyDriver::OpenPort()
{
	this->fdKey = open( "/dev/input/event0", O_RDONLY );
	if ( this->fdKey < 0 )
	{
		this->fdKey = 0;
		return errno;
	}

	return 0;
}

void GPIOKeyDriver::ClosePort()
{
	close( this->fdKey );
}

void GPIOKeyDriver::SetCurrentValue( struct input_event newValue )
{
	list<EventParameter>* parameters = new list<EventParameter>();
	parameters->push_back( EventParameter( GPIOKEY_CODE_PARAM, newValue.type, "int" ) );
	parameters->push_back( EventParameter( GPIOKEY_TYPE_PARAM, newValue.code, "int" ) );
	parameters->push_back( EventParameter( GPIOKEY_VALUE_PARAM, newValue.value, "int" ) );

	Event event( CHVAL_EVENT, parameters );

	//Log
	EventLogger::Instance()->WriteVerbose( "GPIOKeyDriver - StatusChangedEvent Key Code: %d Value: %d",
			newValue.code, newValue.value );

	this->StatusChangedEvent( this, event );
}

struct input_event GPIOKeyDriver::ReadValue()
{
	struct input_event ev;

	while ( true )
	{
		if( this->shouldStop == true )
			break;

		this->nBytes = read( this->fdKey, (char *) &ev,
				sizeof(struct input_event) );

		if ( ev.code == this->keyCode )
			break;
	}

	return ev;
}

void* ReadKeyThreadRun( void* param )
{
	GPIOKeyDriver* driver = (GPIOKeyDriver*) param;

	struct input_event newValues;

	while ( true )
	{
		newValues = driver->ReadValue();

		pthread_mutex_lock( &driver->commonResourcesMutex );
		if(driver->shouldStop == true)
			break;
		pthread_mutex_unlock( &driver->commonResourcesMutex );

		driver->SetCurrentValue( newValues );
	}

	pthread_exit( (void *) 0 );
}

void GPIOKeyDriver::StartReadingKeyThread()
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
			ReadKeyThreadRun, this );

	if ( returnStatus != 0 )
	{
		EventLogger::Instance()->WriteError( ((string)("Error creating GPIOKey driver read key thread!")).c_str() );
		return;
	}
}

void GPIOKeyDriver::StopReadingKeyThread()
{
	pthread_mutex_lock( &this->commonResourcesMutex );
	this->shouldStop = true;
	pthread_mutex_unlock( &this->commonResourcesMutex );

	this->ClosePort();

	//pthread_join( this->readKeyThread, NULL );

	string logMessage = "GPIOKeyDriver - Stop thread succeeded. ";
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );
}

void GPIOKeyDriver::UpdateInitialState()
{
	// input_event initialValue = this->ReadValue();
	// this->SetCurrentValue( initialValue );
}
