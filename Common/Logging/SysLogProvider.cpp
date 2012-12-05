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

#include "SysLogProvider.h"
#include <sys/klog.h>
#include <iostream>

using namespace std;

SysLogProvider::SysLogProvider( const char * name )
{
	this->name = name;
}

SysLogProvider::~SysLogProvider()
{
	// TODO Auto-generated destructor stub
}

void SysLogProvider::OpenLog()
{
	openlog( this->name, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1 );
}

void SysLogProvider::CloseLog()
{
	closelog();
}

void SysLogProvider::ClearLog()
{
	int logSize = klogctl(10, NULL, 0);
	logSize = klogctl(5, NULL, 0);
	logSize = klogctl(10, NULL, 0);
}

void SysLogProvider::WriteInformation( const char *message )
{
	syslog( LOG_INFO, "%s", message );
}

void SysLogProvider::WriteNotice( const char *message )
{
	syslog( LOG_NOTICE, "%s", message );
}

void SysLogProvider::WriteWarning( const char *message )
{
	syslog( LOG_WARNING, "%s", message );
}

void SysLogProvider::WriteCritical( const char *message )
{
	syslog( LOG_CRIT, "%s", message );
}

void SysLogProvider::WriteError( const char *message )
{
	syslog( LOG_NOTICE, "%s", message );
}

void SysLogProvider::WriteEmergency( const char *message )
{
	syslog( LOG_EMERG, "%s", message );
}

void SysLogProvider::WriteAlert( const char *message )
{
	syslog( LOG_ALERT, "%s", message );
}

void SysLogProvider::WriteDebug( const char *message )
{
	syslog( LOG_DEBUG, "%s", message );
}
