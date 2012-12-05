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

#include "JsonLogProvider.h"
#include "time.h"
#include "../Configuration.h"
#include "../Consts.h"

using namespace std;

JsonLogProvider::JsonLogProvider()
{
	this->messages = new list<string>();

	this->maxMessagesCount = Configuration::Instance()->GetIntParam(CONFIG_MAXLOGSIZE);
	if( this->maxMessagesCount == 0 )
		this->maxMessagesCount = MAX_DefaultMessagesCount;

}

JsonLogProvider::~JsonLogProvider()
{
	if( this->messages != NULL )
		delete this->messages;
}

void JsonLogProvider::OpenLog()
{
}

void JsonLogProvider::CloseLog()
{
}

void JsonLogProvider::WriteInformation( const char *message )
{
	this->WriteMessage( (string)message, "Information" );
}

void JsonLogProvider::WriteNotice( const char *message )
{
	this->WriteMessage( (string)message, "Notice" );
}

void JsonLogProvider::WriteWarning( const char *message )
{
	this->WriteMessage( (string)message, "Warning" );
}

void JsonLogProvider::WriteCritical( const char *message )
{
	this->WriteMessage( (string)message, "Critical" );
}

void JsonLogProvider::WriteError( const char *message )
{
	this->WriteMessage( (string)message, "Error" );
}

void JsonLogProvider::WriteEmergency( const char *message )
{
	this->WriteMessage( (string)message, "Emergency" );
}

void JsonLogProvider::WriteAlert( const char *message )
{
	this->WriteMessage( (string)message, "Alert" );
}

void JsonLogProvider::WriteDebug( const char *message )
{
	this->WriteMessage( (string)message, "Debug" );
}

void JsonLogProvider::WriteMessage( string message, string type )
{
	time_t now;
	time( &now );
	string currentTime = ctime(&now);

	string jsonMessage = "{'" + (string)LOG_ItemType + "':'" + type + "','" +
			(string)LOG_ItemMessage + "':'" + message + "','" +
			(string)LOG_ItemTimestamp + "':'" + currentTime + "'}";

	this->messages->push_front(jsonMessage);

	while( this->messages->size() > this->maxMessagesCount  )
		this->messages->pop_back();
}

string JsonLogProvider::ToJson()
{
	string result = "[";

	list<string>::iterator it;
	for( it = this->messages->begin(); it != this->messages->end(); it++ )
	{
		result += (*it);
		result += ",";
	}

	if( result.length() > 1 )
		result = result.substr( 0, result.length() - 1 );

	result += "]";

	return result;
}
