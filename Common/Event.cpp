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

#include "Event.h"
#include <iostream>
#include <sstream>
#include <string.h>
#include "JsonParameters.h"

using namespace std;

Event::Event()
{
	this->name = "";
	this->parameters = new list<EventParameter>();
}

Event::Event( string name )
{
	this->name = name;
	this->parameters = new list<EventParameter>();
}

Event::Event( string name, list<EventParameter>* params )
{
	this->name = name;
	this->parameters = params;
}

Event::Event( const Event& param )
{
	this->name = param.name;
	this->parameters = new list<EventParameter>();

	if(param.parameters != NULL)
	{
		list<EventParameter>::iterator it;
		for( it = param.parameters->begin(); it != param.parameters->end(); it++ )
		{
			this->parameters->push_back( (*it).Clone() );
		}
	}
}

Event::~Event()
{
	if ( this->parameters != NULL )
		delete this->parameters;
}

string Event::Name()
{
	return this->name;
}

list<EventParameter>* Event::Parameters()
{
	return this->parameters;
}

void Event::AddParameter( EventParameter param )
{
	this->parameters->push_back( param );
}

EventParameter Event::GetParameter( string paramName )
{
	list<EventParameter>::iterator i;

	for ( i = this->parameters->begin(); i != this->parameters->end(); i++ )
	{
		if ( !strcmp( ( *i ).Key, paramName.c_str() ) )
			return ( *i ).Clone();
	}

	throw( "Parameter not found" );
}

EventDescriptor Event::GetDescriptor()
{
	EventDescriptor event;

	PropertyDescriptor eventName( "Name", "string", this->name );
	PropertyDescriptor eventDisplayName( "DisplayName", "string", this->name );
	event.Properties.push_back( eventName );
	event.Properties.push_back( eventDisplayName );

	stringstream out;

	if( this->parameters == NULL )
		return event;

	list<EventParameter>::iterator it;
	for(it = this->parameters->begin(); it != this->parameters->end(); it++)
	{
		out.clear();
		out << (*it).Value;

		EventParameterDescriptor evParam;
		PropertyDescriptor propKey( "Key", "string" , (string)(*it).Key );
		PropertyDescriptor propName( "DisplayName", "string" , (string)(*it).Key );
		PropertyDescriptor propType( "Type", "string", (string)(*it).Type );
		PropertyDescriptor propValue( "Value", "string", out.str() );

		evParam.Properties.push_back( propKey );
		evParam.Properties.push_back( propName );
		evParam.Properties.push_back( propType );
		evParam.Properties.push_back( propValue );

		event.EventParameters.push_back( evParam );
	}

	return event;
}

Event* Event::Clone()
{
	list<EventParameter>* params = new list<EventParameter> ();

	list<EventParameter>::iterator i;

	for ( i = this->parameters->begin(); i != this->parameters->end(); i++ )
	{
		EventParameter param = ( *i ).Clone();
		params->push_back( param );
	}

	Event* clone = new Event( this->Name(), params );
	return clone;
}

string Event::ToJson()
{
    string jsonDescr = "{\"" + JsonParameters::Name + "\": \"" + this->name + "\", \""
        + JsonParameters::DisplayName + "\": \"" + this->name + "\",\"" + JsonParameters::EventParameters + "\": [";

    int i = 0;
    int parametersCount = this->parameters->size();
    list<EventParameter>::iterator it;

    for ( it = this->parameters->begin(); it != this->parameters->end(); it++ )
    {
        i++;
        if ( i != parametersCount )
            jsonDescr += (*it).ToJson() + ", ";
        else
            jsonDescr += (*it).ToJson();
    }

    jsonDescr += "] }";
    return jsonDescr;
}
