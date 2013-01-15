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

#include "PinoutMapping.h"

PinoutMapping::PinoutMapping( int pinoutId, int deviceId, string driverType )
{
	this->pinoutId = pinoutId;
	this->deviceId = deviceId;
	this->driverType = driverType;

	this->commands = new list<CommandMapping*>();
	this->events = new list<EventMapping*>();
}

PinoutMapping::~PinoutMapping()
{
	if( this->commands != NULL )
	{
		list<CommandMapping*>::iterator cmdIt;
		for( cmdIt = this->commands->begin(); cmdIt != this->commands->end(); cmdIt++ )
		{
			if( *cmdIt != NULL )
				delete *cmdIt;
		}
	}

	if( this->events != NULL )
	{
		list<EventMapping*>::iterator evIt;
		for( evIt = this->events->begin(); evIt != this->events->end(); evIt++ )
		{
			if( *evIt != NULL )
				delete *evIt;
		}
	}

	if( this->commands != NULL )
		delete this->commands;

	if( this->events != NULL )
		delete this->events;
}

list<CommandMapping*>* PinoutMapping::Commands()
{
	return this->commands;
}

list<EventMapping*>* PinoutMapping::Events()
{
	return this->events;
}

string PinoutMapping::DriverType()
{
	return this->driverType;
}

int PinoutMapping::PinoutId()
{
	return this->pinoutId;
}

int PinoutMapping::DeviceId()
{
	return this->deviceId;
}

CommandMapping* PinoutMapping::GetCommandMapping( string sourceName )
{
	CommandMapping* sourceMapping;
	list<CommandMapping*>::iterator i;

	for( i = this->commands->begin(); i != this->commands->end(); i++ )
	{
		if( (*i)->Source() == sourceName )
		{
			sourceMapping = *i;
			return sourceMapping;
		}
	}

	return NULL;
}

EventMapping* PinoutMapping::GetEventMapping( string sourceName )
{
	EventMapping* sourceMapping;
	list<EventMapping*>::iterator i;

	for( i = this->events->begin(); i != this->events->end(); i++ )
	{
		if( (*i)->Target() == sourceName )
		{
			sourceMapping = *i;
			return sourceMapping;
		}
	}

	cout<<"Event with name "<<sourceName<<" is not available!\n";
	return NULL;
}
