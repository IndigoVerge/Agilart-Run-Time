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

#include "BaseDriver.h"
#include <string>
#include "JsonParameters.h"

using namespace std;

BaseDriver::BaseDriver()
{
	this->commands = new list<Command>();

	this->supportedComands = new list<Command>();
	this->suportedEvents = new list<Event>();
}

BaseDriver::~BaseDriver()
{
	if(this->commands != NULL)
		delete this->commands;

	if(this->suportedEvents != NULL)
		delete this->suportedEvents;

	if(this->supportedComands != NULL)
		delete this->supportedComands;
}

DriverDescriptor BaseDriver::GetDescriptor()
{
	DriverDescriptor driverDescriptor;


	PropertyDescriptor nameProperty( "Name", "string", this->Type());
	PropertyDescriptor displayNameProperty( "DisplayName", "string", this->Name());

	driverDescriptor.Properties.push_back( nameProperty );
	driverDescriptor.Properties.push_back( displayNameProperty );

	list<Command>::iterator ci;
	for(ci = this->supportedComands->begin(); ci != this->supportedComands->end(); ci++)
		driverDescriptor.SupportedCommands.push_back((*ci).GetDescriptor());

	list<Event>::iterator ei;
	for(ei = this->suportedEvents->begin(); ei != this->suportedEvents->end(); ei++)
		driverDescriptor.SupportedEvents.push_back((*ei).GetDescriptor());

	return driverDescriptor;
}

string BaseDriver::ToJson()
{
	string jsonDescr = "{\"" + JsonParameters::Name + "\":\"" + this->Name() + "\",\""
	                + JsonParameters::DisplayName + "\":\"" + this->Name() + "\",\"" + JsonParameters::Commands + "\":[";

	int i = 0;
	int cmdsCount = this->supportedComands->size();
	list<Command>::iterator cmdIt;

	for ( cmdIt = this->supportedComands->begin(); cmdIt != this->supportedComands->end(); cmdIt++ )
	{
		i++;
		if ( i != cmdsCount )
			jsonDescr += (*cmdIt).ToJson() + ", ";
		else
			jsonDescr += (*cmdIt).ToJson();
	}

	jsonDescr += "],\"" + JsonParameters::Events + "\":[";
	i = 0;
	int eventsCount = this->suportedEvents->size();
	list<Event>::iterator eventsIt;

	for ( eventsIt = this->suportedEvents->begin(); eventsIt != this->suportedEvents->end(); eventsIt++ )
	{
		i++;
		if ( i != eventsCount )
			jsonDescr += (*eventsIt).ToJson() + ", ";
		else
			jsonDescr += (*eventsIt).ToJson();
	}

	jsonDescr += "]}";
	return jsonDescr;
}
