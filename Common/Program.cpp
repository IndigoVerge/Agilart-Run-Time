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
#include "Program.h"

#include "BaseDevice.h"
#include "ConnectionPoint.h"

Program::Program()
{
	this->devices = new list<BaseDevice*> ();
	this->wires = new list<Wire*> ();

}

Program::Program( int id, string name, string descr )
{
	this->id = id;
	this->name = name;
	this->description = descr;
	this->devices = new list<BaseDevice*> ();
	this->wires = new list<Wire*> ();
	this->powerWire = new Wire();
}

Program::~Program()
{
	if ( this->devices != NULL )
	{
		list<BaseDevice*>::iterator it;
		for ( it = this->devices->begin(); it != this->devices->end(); it++ )
		{
			if ( ( *it ) != NULL )
				delete ( *it );
		}

		delete this->devices;
	}

	if ( this->wires != NULL )
	{
		list<Wire*>::iterator wireIt;
		for ( wireIt = this->wires->begin(); wireIt != this->wires->end(); wireIt++ )
		{
			if ( ( *wireIt ) != NULL )
				delete ( *wireIt );
		}

		delete this->wires;
	}

	if(this->powerWire != NULL)
		delete this->powerWire;
}

int Program::Id()
{
	return this->id;
}

string Program::Name()
{
	return this->name;
}

string Program::Description()
{
	return this->description;
}

list<BaseDevice*>* Program::Devices()
{
	return this->devices;
}

void Program::AddDevice( BaseDevice* device )
{
	this->devices->push_back( device );
}

BaseDevice* Program::GetDevice( int id )
{
	list<BaseDevice*>::iterator it;

	for ( it = this->devices->begin(); it != this->devices->end(); it++ )
	{
		if ( ( *it )->Id() == id )
			return *it;
	}

	return NULL;
}

list<Wire*>* Program::Wires()
{
	return this->wires;
}

void Program::AddWire( Wire* wire )
{
	this->wires->push_back( wire );
}

Wire* Program::PowerWire()
{
	return this->powerWire;
}

