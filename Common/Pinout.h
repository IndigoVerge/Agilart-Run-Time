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

#ifndef DEVICEPINOUT_H_
#define DEVICEPINOUT_H_

#include <iostream>
#include <sstream>
#include <list>
#include <string>

#include "BaseDriver.h"
#include "Enums.h"
#include "Event.h"
#include "Logging/EventLogger.h"

using namespace std;

class Pinout
{
	private:
		int id;
		string name;
		string description;

		list<Command>* supportedComands;
		list<Event>* suportedEvents;

	public:
		Pinout();
		Pinout(const Pinout&);
		Pinout(int, string, string, list<Command>*, list<Event>*);
		virtual ~Pinout();

		//triggers the ExecuteCommandEvent used by the Device to do smth
		void ExecuteCommand( Command );
		//caught by the MappingsManager
		void (*ExecuteCommandEvent)( Pinout*, Command );

		//the MappingManager calls this to trigger the StatusChangedEvent
		void EventTriggered( void*, list<EventParameter> );
		//caught by the Device, to perform evaluation logic
		void (*PinoutEventTriggered)( void*, std::list<EventParameter> );

		int Id();
		string Name();
		string Description();

		list<Command> SupportedComands();
		list<Event> SuportedEvents();

		PinoutDescriptor GetDecriptor();
		string ToJson();
};

#endif /* DEVICEPINOUT_H_ */

