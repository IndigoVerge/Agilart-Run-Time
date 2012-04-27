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

#include "GPIOLedDriver.h"
#define DriverName "GPIO Output Driver"

GPIOLedDriver::GPIOLedDriver() : BaseDriver()
{
	Command initCommand( INIT_CMD );
	initCommand.AddParameter( new CommandParameter( PORTNUMBER_KEY, NULL, "int" ) );

	Command setValueCommand( SETVAL_CMD );
	setValueCommand.AddParameter( new CommandParameter( VALUE_KEY, NULL, "int" ) );

	this->commands->push_back( initCommand );
	this->commands->push_back( setValueCommand );

	this->supportedComands->push_back( initCommand );
	this->supportedComands->push_back( setValueCommand );

	Command stopCommand( STOP_CMD );
	this->supportedComands->push_back( stopCommand );
}

GPIOLedDriver::~GPIOLedDriver()
{
}

const char* GPIOLedDriver::Type()
{
	return "LED";
}

const char* GPIOLedDriver::Version()
{
	return "0.0.1";
}

const char* GPIOLedDriver::Name()
{
	return DriverName;
}

const char* GPIOLedDriver::Description()
{
	return "GPIO Led Driver";
}

const char* GPIOLedDriver::Publisher()
{
	return "";
}

void GPIOLedDriver::ExecuteCommand( Command command )
{
	string logMessage = "GPIOLedDriver - Execute Command " + command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	if ( !strcmp( command.Name().c_str(), INIT_CMD ) )
	{
		CommandParameter* portnumberParam = command.GetParameter( PORTNUMBER_KEY );
		this->portNumber = *(int*) ( portnumberParam->Value );
	}
	else if ( !strcmp( command.Name().c_str(), SETVAL_CMD ) )
	{
		CommandParameter* valueParam = command.GetParameter( VALUE_KEY );
		int value = *(int*) ( valueParam->Value );

		if ( ( value == 0 || value == 1 ) && this->portNumber >= 0 )
			this->SetValue( this->portNumber, !value ); // revert the value only for our board!!!
	}
	else if ( !strcmp( command.Name().c_str(), STOP_CMD ) )
	{
		string logMessage = "GPIOLedDriver - Stop succeeded. ";
		EventLogger::Instance()->WriteVerbose( logMessage.c_str() );
	}
	else
	{
		EventLogger::Instance()->WriteVerbose( "The command '%s' is not supported by the GPIOLedDriver.", command.Name().c_str() );
	}
}

list<Command> GPIOLedDriver::Commands()
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

BaseDriver* GPIOLedDriver::Clone()
{
	BaseDriver* clone = new GPIOLedDriver();
	return clone;
}

Command* GPIOLedDriver::GetCommand( const char* name )
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

void GPIOLedDriver::SetValue( int portnumber, int value )
{
	int fd = open( "/dev/leds", 0 );
	int setValueResult = ioctl( fd, value, portnumber );

	if ( setValueResult != 0 )
	{
		EventLogger::Instance()->WriteVerbose( "Set Led GPIO(%d) Value fails to succeed.", this->portNumber );
		perror( "Set Led GPIO Value fails to succeed." );
	}

	this->status = value;
	close(fd);
}
