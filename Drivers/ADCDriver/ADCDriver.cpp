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

#include "ADCDriver.h"
#include "Consts.h"
#include "Logging/EventLogger.h"

#define ADCDriver_NAME "ADC Driver"
#define ADCDriver_TYPE "ADCDriver"
#define ADCDriver_VERSION "0.0.1"
#define ADCDriver_DESCR "ADC Driver Description"
#define ADCDriver_PUBL "CompletIT Ltd."

#define ADCDriver_MINVALUE "Min Value"
#define ADCDriver_MAXVALUE "Max Value"
#define ADCDriver_VALSTEP  "Value Step"
#define ADCDriver_TIMESTEP "Interval"

#define ADCDriver_VALUE "Value"
#define ADCDriver_VALEVENT "Value Changed"

using namespace std;

ADCDriver::ADCDriver() : BaseDriver()
{
	Command initCommand( INIT_CMD );
	initCommand.AddParameter( new CommandParameter( ADCDriver_MINVALUE, NULL, INT_TYPE ) );
	initCommand.AddParameter( new CommandParameter( ADCDriver_MAXVALUE, NULL, INT_TYPE ) );
	initCommand.AddParameter( new CommandParameter( ADCDriver_VALSTEP, NULL, INT_TYPE ) );
	initCommand.AddParameter( new CommandParameter( ADCDriver_TIMESTEP, NULL, INT_TYPE ) );

	this->commands->push_back( initCommand );
	this->supportedComands->push_back( initCommand );

	Event changeValueEvent( ADCDriver_VALEVENT );
	changeValueEvent.AddParameter(EventParameter( ADCDriver_VALUE, 0, INT_TYPE ));

	this->suportedEvents->push_back( changeValueEvent );

	Command stopCommand( STOP_CMD );
	this->supportedComands->push_back( stopCommand );
}

ADCDriver::~ADCDriver()
{
	// implemented in the BaseDriver
}

const char* ADCDriver::Type()
{
	return ADCDriver_VERSION;
}

const char* ADCDriver::Version()
{
	return ADCDriver_TYPE;
}

const char* ADCDriver::Name()
{
	return ADCDriver_NAME;
}

const char* ADCDriver::Description()
{
	return ADCDriver_DESCR;
}

const char* ADCDriver::Publisher()
{
	return ADCDriver_PUBL;
}

void ADCDriver::ExecuteCommand( Command command )
{
	string logMessage = "ADCDriver - Execute Command "	+ command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	if ( !strcmp( command.Name().c_str(), INIT_CMD ) )
	{
		CommandParameter* minValueParam = command.GetParameter( ADCDriver_MINVALUE );
		this->minValue = *(int*)(minValueParam->Value);

		CommandParameter* maxValueParam = command.GetParameter( ADCDriver_MAXVALUE );
		this->maxValue = *(int*)(maxValueParam->Value);

		CommandParameter* stepValueParam = command.GetParameter( ADCDriver_VALSTEP );
		this->changeStep = *(int*)(stepValueParam->Value);

		CommandParameter* timeValueParam = command.GetParameter( ADCDriver_TIMESTEP );
		this->timeInterval = *(int*)(timeValueParam->Value);

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

Command* ADCDriver::GetCommand( const char* name )
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

list<Command> ADCDriver::Commands()
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

BaseDriver* ADCDriver::Clone()
{
	BaseDriver* driver = new ADCDriver();
	return driver;
}

int ADCDriver::OpenPort()
{
	return 0;
}

void ADCDriver::ClosePort()
{
}

void ADCDriver::SetCurrentValue( int newValue )
{
	list<EventParameter>* parameters = new list<EventParameter>();
	parameters->push_back( EventParameter( ADCDriver_VALUE, newValue, INT_TYPE ) );

	Event event( ADCDriver_VALEVENT, parameters );

	//Log
	EventLogger::Instance()->WriteVerbose( "ADCDriver - StatusChangedEvent Value: %f", newValue );

	this->StatusChangedEvent( this, event );
}

void* ReadADCValueThreadRun( void* param )
{
	ADCDriver* driver = (ADCDriver*) param;

	int newValue = driver->minValue;
	clock_t endwait;

	while ( true )
	{
		newValue += 1;

		if( newValue == driver->maxValue )
			newValue = driver->minValue;

		pthread_mutex_lock( &driver->commonResourcesMutex );
		if(driver->shouldStop == true)
			break;
		pthread_mutex_unlock( &driver->commonResourcesMutex );

		driver->SetCurrentValue( newValue );
		endwait = clock() + driver->timeInterval * CLOCKS_PER_SEC;

		while (clock() < endwait) {
			// wait
		}
	}

	pthread_exit( (void *) 0 );
}

void ADCDriver::StartReadingKeyThread()
{
	//cancelThreadMutex
	int returnStatus;
	returnStatus = pthread_mutex_init( &this->commonResourcesMutex, NULL );

	if ( returnStatus != 0 )
	{
		EventLogger::Instance()->WriteError( ((string)("Error creating ADC driver Common Resources Mutex!")).c_str() );
		return;
	}

	returnStatus = pthread_create( &this->readKeyThread, NULL, ReadADCValueThreadRun, this );

	if ( returnStatus != 0 )
	{
		EventLogger::Instance()->WriteError( ((string)("Error creating ADC driver read key thread!")).c_str() );
		return;
	}
}

void ADCDriver::StopReadingKeyThread()
{
	pthread_mutex_lock( &this->commonResourcesMutex );
	this->shouldStop = true;
	pthread_mutex_unlock( &this->commonResourcesMutex );

	this->ClosePort();

	//pthread_join( this->readKeyThread, NULL );

	string logMessage = "ADCDriver - Stop thread succeeded. ";
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );
}

void ADCDriver::UpdateInitialState()
{
	// Get ADC current value
	EventLogger::Instance()->WriteVerbose( "ADCDriver - Initialization" );
	this->SetCurrentValue( this->minValue );
}


