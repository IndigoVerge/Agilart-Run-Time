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

#ifndef JSONLOGPROVIDER_H_
#define JSONLOGPROVIDER_H_

#include "BaseLogProvider.h"
#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>
#include <list>
#include <iostream>

#define LOG_ItemType "Type"
#define LOG_ItemMessage "Message"
#define LOG_ItemTimestamp "Timestamp"
#define MAX_DefaultMessagesCount 1000

class JsonLogProvider : BaseLogProvider
{
	private:
		const char* name;
		std::list<std::string>* messages;
		int maxMessagesCount;

		void WriteMessage( std::string message, std::string type );

	public:
		JsonLogProvider();
		virtual ~JsonLogProvider();

		virtual void OpenLog();
		virtual void CloseLog();

		virtual void WriteEmergency( const char * );
		virtual void WriteAlert( const char * );
		virtual void WriteCritical( const char * );
		virtual void WriteError( const char * );
		virtual void WriteWarning( const char * );
		virtual void WriteNotice( const char * );
		virtual void WriteInformation( const char * );
		virtual void WriteDebug( const char * );

		std::string ToJson();
};

#endif /* JSONLOGPROVIDER_H_ */
