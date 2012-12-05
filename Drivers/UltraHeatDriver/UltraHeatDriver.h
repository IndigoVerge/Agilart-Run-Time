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

#ifndef UltraHeatDRIVER_H_
#define UltraHeatDRIVER_H_

#include <string.h>

#include "BaseDriver.h"
#include "Command.h"
#include "CommandParameter.h"
#include "Consts.h"
#include "Logging/EventLogger.h"
#include "UltraHeatCommands.h"

#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>  /* itoa */
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <termios.h>

#define READ_TRIES_COUNT 10

class UltraHeatDriver : public BaseDriver
{
	private:
		// com port file descriptor
		int comPortFD;
		int comPortNumber;
		std::string comPortName;
		int fdDebug;

		int Open_port( void );
		int Close_port( int );
		int WriteData( string );
		string CreateMessage( string, string );

		void StartReadDataThread();
		void StopReadDataThread();

		bool shouldStopReadDataThread;
		bool shouldWaitForResponse;
		int readResponseTries;
		double responseWaitTime; // in ms

		pthread_t readDataThread;
		pthread_mutex_t commonResourcesMutex;

	public:
		UltraHeatDriver();
		virtual ~UltraHeatDriver();

		virtual const char* Type();
		virtual const char* Version();
		virtual const char* Name();
		virtual const char* Description();
		virtual const char* Publisher();

		virtual void ExecuteCommand( Command );
		virtual Command* GetCommand( const char* );

		virtual std::list<Command> Commands();
		virtual BaseDriver* Clone();

		friend void* ReadDataThreadRun( void* param );
};

#endif /* UltraHeatDRIVER_H_ */
