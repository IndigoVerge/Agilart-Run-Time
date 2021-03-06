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

#ifndef SYSLOG_H_
#define SYSLOG_H_

#include "BaseLogProvider.h"
#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>

class SysLogProvider: BaseLogProvider
{
	private:
		const char* name;

	public:
		SysLogProvider( const char * );
		virtual ~SysLogProvider();

		virtual void OpenLog();
		virtual void CloseLog();
		virtual void ClearLog();

		virtual void WriteEmergency( const char * );
		virtual void WriteAlert( const char * );
		virtual void WriteCritical( const char * );
		virtual void WriteError( const char * );
		virtual void WriteWarning( const char * );
		virtual void WriteNotice( const char * );
		virtual void WriteInformation( const char * );
		virtual void WriteDebug( const char * );
};

#endif /* SYSLOG_H_ */
