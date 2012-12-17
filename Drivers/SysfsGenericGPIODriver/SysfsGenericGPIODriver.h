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

#ifndef SYSFSGENERICGPIODRIVER_H_
#define SYSFSGENERICGPIODRIVER_H_

#include <list>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "BaseDriver.h"
#include "Command.h"
#include "CommandParameter.h"
#include "Consts.h"

class SysfsGenericGPIODriver: public BaseDriver
{
	public:
		SysfsGenericGPIODriver();
		~SysfsGenericGPIODriver();

		virtual const char* Type();
		virtual const char* Version();
		virtual const char* Name();
		virtual const char* Description();
		virtual const char* Publisher();

		virtual void ExecuteCommand( Command );
		virtual Command* GetCommand( const char* );
		virtual std::list<Command> Commands();
		virtual BaseDriver* Clone();

		int GetStatus();
		void SetStatus(int value);
		int GetPortNumber();
		int GetGpioValue();
		void SignalPortstatusChanged();
		int gpioFD;

		friend void* SysfsThreadRun( void* param );

	private:
		int portNumber;
		int status;
		bool shouldStop;

		pthread_t refreshStatusThread;
		pthread_mutex_t commonResourcesMutex;

		void StartPoolingForStatus();
		void StopPoolingForStatus();

		virtual int OpenPort( int, int );
		virtual int ClosePort( int );

		virtual void SetValue( int, int );
		virtual int GetValue( int );

		int gpio_export(unsigned int);
		int gpio_unexport(unsigned int);
		int gpio_set_dir(unsigned int, unsigned int);
		int gpio_set_value(int, int);
		int gpio_get_value(int, int*);
		int gpio_set_edge(unsigned int, char*);
		int gpio_fd_open(unsigned int);
		int gpio_fd_close(int);
};


#endif /* SYSFSGENERICGPIODRIVER_H_ */
