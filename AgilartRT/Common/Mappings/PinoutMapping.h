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

#ifndef PINOUTMAPPING_H_
#define PINOUTMAPPING_H_

#include <string>
#include <list>

#include "CommandMapping.h"
#include "EventMapping.h"

using namespace std;

class PinoutMapping
{
	private:
		string driverType;
		int pinoutId;
		int deviceId;

		list<CommandMapping*>* commands;
		list<EventMapping*>* events;

	public:
		PinoutMapping( int, int, string );
		PinoutMapping( const PinoutMapping& param )
		{
			this->driverType = param.driverType;
			this->pinoutId = param.pinoutId;
			this->deviceId = param.deviceId;

			//this->commands = param.commands; DO NOT DO THIS !!!
			//this->events = param.events;

			this->commands = new list<CommandMapping*>();

			list<CommandMapping*>::iterator it;
			for(it = param.commands->begin(); it != param.commands->end(); it++)
			{
				this->commands->push_back(*it);
			}
		}

		virtual ~PinoutMapping();

		string DriverType();
		int PinoutId();
		int DeviceId();

		list<CommandMapping*>* Commands();
		list<EventMapping*>* Events();

		CommandMapping* GetCommandMapping( string sourceName );
		EventMapping* GetEventMapping( string sourceName );
};

#endif /* PINOUTMAPPING_H_ */
