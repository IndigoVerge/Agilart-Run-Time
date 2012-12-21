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

#include "EventLogger.h"
#include <iostream>
using namespace std;

EventLogger* EventLogger::instance = 0;

EventLogger::EventLogger()
{
	this->predefinedErrorMessages[ERROR_PRSTARTED_ID] = ERROR_PRSTARTED_MSG;
	this->predefinedErrorMessages[ERROR_NOMAPPING_ID] = ERROR_NOMAPPING_MSG;
	this->predefinedErrorMessages[ERROR_LOADINGERROR_ID] = ERROR_LOADINGERROR_MSG;
	this->predefinedErrorMessages[ERROR_PRNOTFOUND_ID] = ERROR_PRNOTFOUND_MSG;
	this->predefinedErrorMessages[ERROR_PRNOTSTARTED_ID] = ERROR_PRNOTSTARTED_MSG;
	this->predefinedErrorMessages[ERROR_DEVFATALERROR_ID] = ERROR_DEVFATALERROR_MSG;
}

EventLogger::~EventLogger()
{
	delete this->instance;
	this->loggers.clear();
}

EventLogger* EventLogger::Instance()
{
	if ( instance == NULL )
	{
		instance = new EventLogger();

		SysLogProvider *mySysLog = new SysLogProvider( "Agilart" );
		instance->jsonLog = new JsonLogProvider();

		instance->Register( mySysLog );
		instance->Register( instance->jsonLog );
	}

	return instance;
}

void EventLogger::Register( void *log )
{
	BaseLogProvider* baselog = (BaseLogProvider *) log;
	this->loggers.push_back( log );

	baselog->OpenLog();
}

void EventLogger::Unregister()
{
	list<void*>::iterator iterator;
	BaseLogProvider* baselog;

	for ( iterator = this->loggers.begin(); iterator != this->loggers.end(); iterator++ )
	{
		baselog = (BaseLogProvider *) ( *iterator );
		baselog->CloseLog();
	}
}

string EventLogger::ToJson()
{
	if( this->jsonLog == NULL )
		return "";

	return this->jsonLog->ToJson();
}

void EventLogger::WriteInformation( const char *message )
{
	list<void*>::iterator iterator;
	BaseLogProvider* baselog;

	for ( iterator = this->loggers.begin(); iterator != this->loggers.end(); iterator++ )
	{
		baselog = (BaseLogProvider *) ( *iterator );
		baselog->WriteInformation( message );
	}
}

void EventLogger::WriteVerbose( const char *format, ... )
{
	list<void*>::iterator iterator;
	BaseLogProvider* baselog;

	va_list args;
	va_start( args, format );

	char buffer[256];
	vsprintf( buffer, format, args );

	for ( iterator = this->loggers.begin(); iterator != this->loggers.end(); iterator++ )
	{
		baselog = (BaseLogProvider *) ( *iterator );
		baselog->WriteInformation( buffer );
	}

    va_end( args );
}

void EventLogger::WriteNotice( const char *message )
{
	list<void*>::iterator iterator;
	BaseLogProvider* baselog;

	for ( iterator = this->loggers.begin(); iterator != this->loggers.end(); iterator++ )
	{
		baselog = (BaseLogProvider *) ( *iterator );
		baselog->WriteNotice( message );
	}
}

void EventLogger::WriteWarning( const char *message )
{
	list<void*>::iterator iterator;
	BaseLogProvider* baselog;

	for ( iterator = this->loggers.begin(); iterator != this->loggers.end(); iterator++ )
	{
		baselog = (BaseLogProvider *) ( *iterator );
		baselog->WriteWarning( message );
	}
}

void EventLogger::WriteCritical( const char *message )
{
	list<void*>::iterator iterator;
	BaseLogProvider* baselog;

	for ( iterator = this->loggers.begin(); iterator != this->loggers.end(); iterator++ )
	{
		baselog = (BaseLogProvider *) ( *iterator );
		baselog->WriteNotice( message );
	}
}

void EventLogger::WriteError( const char *format, ... )
{
	list<void*>::iterator iterator;
	BaseLogProvider* baselog;

	va_list args;
	va_start( args, format );

	char buffer[256];
	vsprintf( buffer, format, args );

	for ( iterator = this->loggers.begin(); iterator != this->loggers.end(); iterator++ )
	{
		baselog = (BaseLogProvider *) ( *iterator );
		baselog->WriteError( buffer );
	}

	va_end( args );
}

void EventLogger::WriteEmergency( const char *message )
{
	list<void*>::iterator iterator;
	BaseLogProvider* baselog;

	for ( iterator = this->loggers.begin(); iterator != this->loggers.end(); iterator++ )
	{
		baselog = (BaseLogProvider *) ( *iterator );
		baselog->WriteNotice( message );
	}
}

void EventLogger::WriteAlert( const char *message )
{
	list<void*>::iterator iterator;
	BaseLogProvider* baselog;

	for ( iterator = this->loggers.begin(); iterator != this->loggers.end(); iterator++ )
	{
		baselog = (BaseLogProvider *) ( *iterator );
		baselog->WriteAlert( message );
	}
}

void EventLogger::WriteDebug( const char *format, ... )
{
	list<void*>::iterator iterator;
	BaseLogProvider* baselog;

	va_list args;
	va_start( args, format );

	char buffer[256];
	vsprintf( buffer, format, args );

	for ( iterator = this->loggers.begin(); iterator != this->loggers.end(); iterator++ )
	{
		baselog = (BaseLogProvider *) ( *iterator );
		baselog->WriteDebug( buffer );
	}

	va_end( args );
}

std::string EventLogger::GetErrorMessage( int code )
{
	map<int,std::string>::iterator i = this->predefinedErrorMessages.find( code );
	if( i == this->predefinedErrorMessages.end())
		return "Undefined error code!";

	return this->predefinedErrorMessages[code];

}


