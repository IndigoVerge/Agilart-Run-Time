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
#include "Configuration.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "JsonParameters.h"

using namespace std;

Configuration* Configuration::instance = 0;

Configuration::Configuration()
{
	// TODO Auto-generated constructor stub

}

Configuration::~Configuration()
{
	// TODO Auto-generated destructor stub
}

Configuration* Configuration::Instance()
{
	if ( instance == NULL )
		instance = new Configuration();

	return instance;
}

void Configuration::Initialize( const char *configFilePath, void(*OnError)( string ) )
{
	if ( instance == NULL )
		instance = new Configuration();

	instance->configParameters.clear();
	instance->configFilePath = configFilePath;

	int lineBaseSize = 1024;
	char line[lineBaseSize];

	char *paramName;
	char *paramValue;
	char *nextToken;

	FILE *file = NULL;
	file = fopen( configFilePath, "r" );

	if ( file == NULL )
	{
		file = fopen( DEFAULT_CONFIG_FILE_PATH, "r" );
	}

	if ( file == NULL )
	{
		if( OnError != NULL )
			OnError( "The loading of the Configuration file has failed!" );

		return;
	}

	while ( fgets( line, lineBaseSize, file ) != NULL )
	{
		/* TODO: Ignore spaces. */
		if ( line[0] == '#' ) // Comemnt lines start with #
		    continue;

		paramName = strtok( line, "=" );
		while ( paramName != NULL )
		{
			paramValue = strtok( NULL, ";" );

			//ignore blank lines
			if ( ( ( nextToken = strtok( NULL, "=;" ) ) == NULL ) || paramName[0] == '\n' )
			{
				break;
			}

			/* helper function to populate the config. */
			instance->SetParam( paramName, paramValue );
		}
	}

	fclose( file );
}

void Configuration::SetParam( const char *name, const char *value )
{
	this->configParameters[(string)name] = (string)value;
}

void Configuration::SaveToConfigFile()
{
	ofstream myfile;
	myfile.open( configFilePath, ios::out | ios::trunc );

	if ( myfile == NULL )
	{
		if( OnError != NULL )
			OnError( "The loading of the Configuration file has failed!" );

		return;
	}

	map<string, string>::iterator it;
	for( it = this->configParameters.begin(); it!= this->configParameters.end(); it++ )
	{
		string property = (*it).first + "=" + (*it).second + ";\n";
		myfile << property;
	}

    myfile.close();
}

string Configuration::GetStringParam( string key )
{
	return this->configParameters[(string)key];
}

int Configuration::GetIntParam( string key )
{
	string stringValue = this->GetStringParam( key );
	if( stringValue == "" )
		return 0;

	int intValue = atoi( stringValue.c_str() );

	return intValue;
}

string Configuration::GetSettingsJsonDescription()
{
	string response = "[";
	string configJsonDescr;
	map<string, string>::iterator it;
	uint i = 0;

	for( it = this->configParameters.begin(); it != this->configParameters.end(); it++ )
	{
		configJsonDescr = "{\"" + JsonParameters::SettingName + "\": \"" + (*it).first + "\", \"" +
				JsonParameters::SettingDataType + "\":\"Int\", \"" + JsonParameters::SettingValue + "\":\"" + (*it).second  + "\"}";

		i++;
		response += configJsonDescr;

		if( i != this->configParameters.size() )
			response += ",";
	}

	response += "]";

	return response;
}

void Configuration::SaveSettings( map<string, string> settings )
{
	map<string, string>::iterator it;

	for( it = settings.begin(); it != settings.end(); it++ )
	{
		if( this->configParameters.find( (*it).first ) != this->configParameters.end() )
		{
			this->configParameters[ (*it).first ] = (*it).second;
		}
	}

	this->SaveToConfigFile();
}
