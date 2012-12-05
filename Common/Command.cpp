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

#include "Command.h"
#include <iostream>
#include <sstream>
#include <string>
#include "Logging/EventLogger.h"
#include "JsonParameters.h"

using namespace std;

Command::Command()
{
	this->name = "";
	this->parameters = new list<CommandParameter*> ();
}

Command::Command( string name )
{
	this->name = name;
	this->parameters = new list<CommandParameter*> ();
}

Command::~Command()
{
	if( this->parameters != NULL )
	{
		if( this->parameters->empty() == false )
		{
			this->parameters->clear();
		}

		delete this->parameters;
		this->parameters = NULL;
	}
}

Command::Command( string name, std::list<CommandParameter*>* params )
{
	this->name = name;
	this->parameters = params;
}

CommandParameter* Command::GetParameter( string key )
{
	list<CommandParameter*>::iterator i;

	for ( i = this->parameters->begin(); i != this->parameters->end(); i++ )
	{
		if ( ( *i )->Key == key )
			return ( *i );
	}

	EventLogger::Instance()->WriteVerbose( "Parameter with key - %s is not available!", key.c_str() );
	cout << "Parameter with key - " << key << " is not available!";

	return NULL;
}

string Command::Name()
{
	return this->name;
}

list<CommandParameter*>* Command::Parameters()
{
	return this->parameters;
}

void Command::AddParameter( CommandParameter* param )
{
	this->parameters->push_back( param );
}

CommandDescriptor Command::GetDescriptor()
{
	CommandDescriptor command;

	PropertyDescriptor commandName( "Name", "string", this->name );
	PropertyDescriptor commandDisplayName( "DisplayName", "string", this->name );
	command.Properties.push_back(commandName);
	command.Properties.push_back(commandDisplayName);

	stringstream out;

	if( this->parameters == NULL )
		return command;

	list<CommandParameter*>::iterator it;
	for(it = this->parameters->begin(); it != this->parameters->end(); it++)
	{
		out.clear();
		out << (*it)->Value;

		CommandParameterDescriptor cmdParam;
		PropertyDescriptor propKey( "Key", "string" , (string)(*it)->Key );
		PropertyDescriptor propName( "DisplayName", "string" , (string)(*it)->Key );
		PropertyDescriptor propType( "Type", "string", (string)(*it)->Type );
		PropertyDescriptor propValue( "Value", "string", out.str() );

		cmdParam.Properties.push_back( propKey );
		cmdParam.Properties.push_back( propName );
		cmdParam.Properties.push_back( propType );
		cmdParam.Properties.push_back( propValue );

		command.CommandParameters.push_back( cmdParam );
	}

	return command;
}

Command* Command::Clone()
{
	Command* clone = new Command( this->name );

	std::list<CommandParameter*>::iterator i;

	for ( i = this->parameters->begin(); i != this->parameters->end(); i++ )
	{
		clone->AddParameter( (*i)->Clone() );
	}

	return clone;
}

string Command::ToJson()
{
    string jsonDescr = "{\"" + JsonParameters::Name + "\": \"" + this->name + "\", \""
        + JsonParameters::DisplayName + "\": \"" + this->name + "\",\"" + JsonParameters::CmdParameters + "\": [";

    int i = 0;
    int parametersCount = this->parameters->size();
    list<CommandParameter*>::iterator it;

    for ( it = this->parameters->begin(); it != this->parameters->end(); it++ )
    {
        i++;
        if ( i != parametersCount )
            jsonDescr += (*it)->ToJson() + ", ";
        else
            jsonDescr += (*it)->ToJson();
    }

    jsonDescr += "] }";
    return jsonDescr;
}
