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

#ifndef RASPIGPINDRIVER_H_
#define RASPIGPINDRIVER_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <sys/ioctl.h>
#include <linux/input.h>
#include <string.h>

#include "BaseDriver.h"

class RaspiGPInputDriver: public BaseDriver
{
	public:
		RaspiGPInputDriver();
		virtual ~RaspiGPInputDriver();

		virtual const char* Type();
		virtual const char* Version();
		virtual const char* Name();
		virtual const char* Description();
		virtual const char* Publisher();

		virtual void ExecuteCommand( Command );
		virtual Command* GetCommand( const char* );
		virtual std::list<Command> Commands();
		virtual BaseDriver* Clone();

		int ReadValue();
		void SetCurrentValue(int newValue);

		friend void* RaspiReadKeyThreadRun( void* param );

	private:
		int currentValue;
		int keyCode;
		bool shouldStop;

		pthread_t readKeyThread;
		pthread_mutex_t commonResourcesMutex;

		int OpenPort();
		void ClosePort();

		void StartReadingKeyThread();
		void StopReadingKeyThread();

		void UpdateInitialState();
};

#endif /* GPIOKEYDRIVER_H_ */
