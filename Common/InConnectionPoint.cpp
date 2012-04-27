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
#include "InConnectionPoint.h"
#include "OutConnectionPoint.h"

#include <stdio.h>

InConnectionPoint::InConnectionPoint( int id, string name, string description,
		string dataType, void* currentValue ) :
			ConnectionPoint( id, name, DIRECTION_IN, description, dataType,
					currentValue )
{
	this->handler = NULL;
	this->refreshHandler = NULL;
}

InConnectionPoint::InConnectionPoint( int id, string name, string description,
		AgilartDataTypes dataType, void* currentValue ) :
			ConnectionPoint( id, name, DIRECTION_IN, description, dataType,
					currentValue )
{
	this->handler = NULL;
	this->refreshHandler = NULL;
}

InConnectionPoint::~InConnectionPoint()
{
	// TODO:
	//if ( this->currentValue != NULL )
		//delete this->currentValue;
}

void InConnectionPoint::SetValue( void* newValue )
{
	if ( Convert::AreEqual( this->currentValue, newValue, this->dataType ) == true && this->isInitialized == true )
		return;

	this->currentValue = newValue;
	this->isInitialized = true;

	this->TriggerValueChanged();
}

ConnectionPoint* InConnectionPoint::Clone()
{
	void* value = NULL;
	bool isInitialized = false;

	if(this->currentValue == NULL)
	{
	}
	else
	{
		switch( this->dataType )
		{
			case INT:
				value = new int( Convert::ToInt( this->currentValue, this->dataType ) );
				break;
			case STRING:
				value = new string( Convert::ToString( this->currentValue, this->dataType ) );
				break;
			case FLOAT:
				value = new float( Convert::ToFloat( this->currentValue, this->dataType ) );
				break;
		}

		isInitialized = true;
	}

	ConnectionPoint* clone = new InConnectionPoint( this->id, this->name,
			this->description, this->dataType, value );

	clone->SetIsCalculated( this->isCalculated );
	clone->SetIsInitialized( isInitialized );

	return clone;
}

void InConnectionPoint::RegisterValueChangedHandler( IValueChanged* handler )
{
	this->InValueChanged = &IValueChanged::InputPinValueChanged;
	this->handler = handler;
}

void InConnectionPoint::TriggerValueChanged()
{
	if( this->handler != NULL )
	{
		(this->handler->*InValueChanged)( this );
	}
}

void InConnectionPoint::RegisterRefreshHandler( IRefreshValue* handler )
{
	this->RefreshValue = &IRefreshValue::Refresh;
	this->refreshHandler = handler;
}

void InConnectionPoint::Refresh()
{
	if( this->refreshHandler != NULL )
	{
		(this->refreshHandler->*RefreshValue)( this );
	}
}
