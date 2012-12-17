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
#include <sstream>
#include <stdio.h>
#include "ConnectionPoint.h"

using namespace std;

ConnectionPoint::ConnectionPoint( int id, string name,
		PinoutDirection direction, string description, string dataType,
		void* currentValue )
{
	this->id = id;
	this->name = name;
	this->direction = direction;
	this->description = description;
	this->dataType = Convert::ToType( dataType );
	this->currentValue = currentValue;

	this->isCalculated = false;
	this->isInitialized = false;
}

ConnectionPoint::ConnectionPoint( int id, string name,
		PinoutDirection direction, string description, AgilartDataTypes dataType,
		void* currentValue )
{
	this->id = id;
	this->name = name;
	this->direction = direction;
	this->description = description;
	this->dataType = dataType;
	this->currentValue = currentValue;

	this->isCalculated = false;
	this->isInitialized = false;
}

ConnectionPoint::~ConnectionPoint()
{
	//if ( this->currentValue != NULL )
		//delete this->currentValue;
}

std::string ConnectionPoint::Name()
{
	return this->name;
}

int ConnectionPoint::Id()
{
	return this->id;
}

AgilartDataTypes ConnectionPoint::DataType()
{
	return this->dataType;
}

bool ConnectionPoint::IsInitialized()
{
	return this->isInitialized;
}

bool ConnectionPoint::IsCalculated()
{
	return this->isCalculated;
}

void ConnectionPoint::ConnectionPointInitialized()
{
	this->isCalculated = true;
}

void ConnectionPoint::SetDataType( AgilartDataTypes newDataType )
{
	this->dataType = newDataType;
}

void ConnectionPoint::SetIsInitialized(bool value)
{
	this->isInitialized = value;
}

void ConnectionPoint::SetIsCalculated(bool value)
{
	this->isCalculated = value;
}

PinoutDirection ConnectionPoint::Direction()
{
	return this->direction;
}

std::string ConnectionPoint::Description()
{
	return this->description;
}

void* ConnectionPoint::Value()
{
	return this->currentValue;
}

void ConnectionPoint::SetValue( void* )
{
	// Implemented in the derived classes;
}

ConnectionPoint* ConnectionPoint::Clone()
{
	ConnectionPoint* cp = new ConnectionPoint(this->id, this->name,
			this->direction, this->description, this->dataType, NULL );

	return cp;
}

void ConnectionPoint::RegisterValueChangedHandler( IValueChanged* handler )
{
	// Implemented in the derived classes;
}

void ConnectionPoint::TriggerValueChanged()
{
	// Implemented in the derived classes;
}

void ConnectionPoint::RegisterRefreshHandler( IRefreshValue* handler )
{
	// Implemented in the derived classes;
}

void ConnectionPoint::Refresh()
{
	// Implemented in the derived classes;
}

string ConnectionPoint::ToJson()
{
	char connPointId[16];
	sprintf( connPointId, "%d", this->Id() );

    string jsonDescr = "{\"" + JsonParameters::Id + "\": \"" + (string)connPointId + "\", \""
       + JsonParameters::Direction + "\": \""
       + ( this->Direction() == DIRECTION_IN ?
    		   JsonParameters::InConnPointDirection : JsonParameters::OutConnPointDirection ) + "\", \""
       + JsonParameters::ConnPointLabel + "\": \"" + this->Name() + "\"}";

    return jsonDescr;
}

PinState ConnectionPoint::GetState()
{
	return this->state;
}

void ConnectionPoint::SetState( PinState newState )
{
	this->state = newState;
}
