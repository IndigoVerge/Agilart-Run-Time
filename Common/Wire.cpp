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
#include "Wire.h"

#include <iostream>
using namespace std;

Wire::Wire()
{
	this->connectionPoints = new list<ConnectionPoint *> ();
}

Wire::~Wire()
{
	if ( this->connectionPoints == NULL )
		return;

	list<ConnectionPoint*>::iterator it;
	for ( it = this->connectionPoints->begin(); it
			!= this->connectionPoints->end(); it++ )
	{
		if ( ( *it ) != NULL )
			delete ( *it );
	}

	delete this->connectionPoints;
}

list<ConnectionPoint*>* Wire::ConnectionPoints()
{
	return this->connectionPoints;
}

void Wire::Attach( ConnectionPoint *connectionPoint )
{
	this->connectionPoints->push_back( connectionPoint );

	if(connectionPoint->Direction() == DIRECTION_OUT)
		connectionPoint->RegisterValueChangedHandler( this );

	if(connectionPoint->Direction() == DIRECTION_IN)
		connectionPoint->RegisterRefreshHandler( this );
}

void Wire::Detach( ConnectionPoint *target )
{
	list<ConnectionPoint*>::iterator it = find(
			this->connectionPoints->begin(),
			this->connectionPoints->end(),
			target );

	if ( it == this->connectionPoints->end() )
		return; // The specified element is not in the list of junctions.

	this->connectionPoints->remove( target );
}

void Wire::InputPinValueChanged( void* connPointPtr )
{
	ConnectionPoint* changedConnPoint = (ConnectionPoint*)connPointPtr;

	if( changedConnPoint->Direction() == DIRECTION_IN )
	{
		throw "no INs here... TODO - LOG";
	}

	void* value = changedConnPoint->Value();
	AgilartDataTypes senderType = changedConnPoint->DataType();

	this->SetValue( value, senderType );
}

void Wire::SetValue(void* value, AgilartDataTypes valueType)
{
	list<ConnectionPoint*>::iterator i;

	for ( i = this->connectionPoints->begin(); i != this->connectionPoints->end(); i++ )
	{
		if ( ( *i )->Direction() == DIRECTION_OUT )
			continue;

		AgilartDataTypes receiverType = (*i)->DataType();
		InConnectionPoint* receiver = (InConnectionPoint *) ( *i );

		if( receiverType == ANY )
		{
			receiver->SetDataType( valueType );
			receiver->SetValue( value );
		}
		else
		{
			void* targetValue = Convert::ToTypeInstance( value, valueType, receiverType );
			receiver->SetValue( targetValue );
		}
	}
}

void Wire::Refresh( void* connPointPtr )
{
	InConnectionPoint* sender = ( InConnectionPoint* )connPointPtr;

	if( sender == NULL )
	{
		throw "no OUTs here... TODO - LOG";
	}

	ConnectionPoint* outConnPoint = NULL; // SHOULD BE EXACTLY ONE IN EACH WIRE
	list<ConnectionPoint*>::iterator i;

	for ( i = this->connectionPoints->begin(); i != this->connectionPoints->end(); i++ )
	{
		if ( ( *i )->Direction() == DIRECTION_OUT )
		{
			outConnPoint = *i;
			break;
		}
	}

	if( outConnPoint == NULL )
	{
		throw "No OUT in the Wire! The program is invalid and cannot be executed.";
	}

	outConnPoint->Refresh();

	void* value = outConnPoint->Value();
	AgilartDataTypes senderType = outConnPoint->DataType();

	this->SetValue( value, senderType );
}

WireDescriptor Wire::GetDescriptor()
{
	WireDescriptor wire;
	return wire;
}

Wire* Wire::Clone()
{
	Wire* clone = new Wire();
	list<ConnectionPoint *>::iterator it;

	for( it = this->connectionPoints->begin(); it != this->connectionPoints->end(); it++ )
	{
		ConnectionPoint* cp = (*it)->Clone();
		clone->Attach( cp );
	}

	return clone;
}
