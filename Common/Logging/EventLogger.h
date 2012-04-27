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

#ifndef EVENTLOG_H_
#define EVENTLOG_H_

#include "BaseLogProvider.h"
#include "SysLogProvider.h"
#include "JsonLogProvider.h"

#include <list>
#include <map>
#include <stdarg.h>

#define ERROR_PRSTARTED_MSG "The program is already started."
#define ERROR_PRSTARTED_ID 1

#define ERROR_NOMAPPING_MSG "The program mappings file is not available."
#define ERROR_NOMAPPING_ID 2

#define ERROR_LOADINGERROR_MSG "Error while loading the program from the json description file."
#define ERROR_LOADINGERROR_ID 3

#define ERROR_PRNOTFOUND_MSG "The program is not available."
#define ERROR_PRNOTFOUND_ID 4

#define ERROR_PRNOTSTARTED_MSG "The program is not started."
#define ERROR_PRNOTSTARTED_ID 5

#define ERROR_DEVFATALERROR_MSG "Fatal error in a program widget. Check the Log."
#define ERROR_DEVFATALERROR_ID 3

class EventLogger
{
	private:
		static EventLogger * instance;
		std::list<void*> loggers;
		std::map<int,std::string> predefinedErrorMessages;
		JsonLogProvider *jsonLog;

	public:
		EventLogger();
		virtual ~EventLogger();

		static EventLogger* Instance();

		void Register( void * log );
		void Unregister();

		virtual void WriteEmergency( const char * );
		virtual void WriteAlert( const char * );
		virtual void WriteCritical( const char * );
		virtual void WriteError( const char * );
		virtual void WriteWarning( const char * );
		virtual void WriteNotice( const char * );
		virtual void WriteInformation( const char * );
		virtual void WriteDebug( const char * );

		void WriteVerbose( const char *message, ... );

		std::string GetErrorMessage( int code );
		std::string ToJson();
};

#endif /* EVENTLOG_H_ */
