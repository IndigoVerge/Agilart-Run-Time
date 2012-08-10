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

#ifndef RASPIGPIOUTDRIVER_H_
#define RASPIGPIOUTDRIVER_H_

#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "BaseDriver.h"
#include "Command.h"
#include "CommandParameter.h"
#include "Consts.h"
#include "Logging/EventLogger.h"

class RaspiGPOutputDriver : public BaseDriver
{
public:
	RaspiGPOutputDriver();
	virtual ~RaspiGPOutputDriver();

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

private:
	int portNumber; // 0,1,2 or 3
	int status; // 0 = ON, 1 = OFF

	virtual void SetValue( int, int );
};

#endif /* GPIOLEDDRIVER_H_ */
