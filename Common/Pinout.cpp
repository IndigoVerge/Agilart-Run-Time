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

#include <iostream>
#include "Pinout.h"

using namespace std;

Pinout::Pinout()
{
	this->supportedComands = new list<Command>();
	this->suportedEvents = new list<Event>();
}

Pinout::Pinout( const Pinout &copy )
{
	this->id = copy.id;
	this->name = copy.name;
	this->description = copy.description;
}

Pinout::Pinout( int id, string name, string description, list<Command>* commands, list<Event>* events )
{
	this->id = id;
	this->name = name;
	this->description = description;

	this->supportedComands = commands;
	this->suportedEvents = events;
}

Pinout::~Pinout()
{
	if ( this->supportedComands != NULL )
		delete this->supportedComands;

	if ( this->suportedEvents != NULL )
		delete this->suportedEvents;
}

int Pinout::Id()
{
	return this->id;
}

std::string Pinout::Name()
{
	return this->name;
}

std::string Pinout::Description()
{
	return this->description;
}

void Pinout::ExecuteCommand( Command command )
{
	if( this->ExecuteCommandEvent != NULL )
		this->ExecuteCommandEvent( this, command );
}

void Pinout::EventTriggered( void* sender, list<EventParameter> params )
{
	EventLogger::Instance()->WriteInformation( "Pinout::EventTriggered" );
	// the device is hooked on this event
	if( PinoutEventTriggered != NULL )
		this->PinoutEventTriggered( sender, params );
}

list<Command> Pinout::SupportedComands()
{
	return *this->supportedComands;
}

list<Event> Pinout::SuportedEvents()
{
	return *this->suportedEvents;
}

string Pinout::ToJson()
{
	char pinoutId[16];
	sprintf( pinoutId, "%d", this->Id() );

	 string jsonDescr = "{\"" + JsonParameters::Id + "\": \"" + (string)pinoutId + "\", \""
	   + JsonParameters::Commands + "\": [";

	int i = 0;
	int count = this->supportedComands->size();

	list<Command>::iterator cmdIt;
	list<Event>::iterator eventIt;

	for ( cmdIt = this->supportedComands->begin(); cmdIt != this->supportedComands->end(); cmdIt++ )
	{
		i++;
		if ( i != count )
			jsonDescr += (*cmdIt).ToJson() + ", ";
		else
			jsonDescr += (*cmdIt).ToJson();
	}

	jsonDescr += "], \"" + JsonParameters::DisplayName + "\": \"" +
		this->Name() + "\", \"" + JsonParameters::Events + "\": [";

	i = 0;
	count = this->suportedEvents->size();

	for ( eventIt = this->suportedEvents->begin(); eventIt != this->suportedEvents->end(); eventIt++ )
	{
		i++;
		if ( i != count )
			jsonDescr += (*eventIt).ToJson() + ", ";
		else
			jsonDescr += (*eventIt).ToJson();
	}

	jsonDescr += "]}";
	return jsonDescr;
}
