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
#include "PluggableDriversProvider.h"

using namespace std;

PluggableDriversProvider::PluggableDriversProvider()
{
	this->plugindir = "./plugins";
}

PluggableDriversProvider::~PluggableDriversProvider()
{
}

list<BaseDriver*> PluggableDriversProvider::Drivers()
{
	list<BaseDriver*> driversList;
	DriversMap::iterator it;

	for ( it = this->drivers.begin(); it != this->drivers.end(); it++ )
		driversList.push_back( it->second );

	return driversList;
}

bool PluggableDriversProvider::SupportsDriver( const char * type )
{
	DriversMap::iterator iter = this->drivers.find( type );
	return iter == this->drivers.end();
}

BaseDriver* PluggableDriversProvider::GetDriver( const char * type )
{
	map<const char*, BaseDriver*>::iterator it;

	for( it = this->drivers.begin(); it != this->drivers.end(); it++ )
	{
		if( !strcmp((it)->first, type) )
			return ((it)->second)->Clone();
	}

	return NULL;
}

int PluggableDriversProvider::GetDlError( const char *message )
{
	char *error;
	error = dlerror();

	if ( error != 0 )
	{
		EventLogger::Instance()->WriteCritical( error );
		cout << message << endl << error << endl;
		return 0;
	}

	return 1;
}

bool PluggableDriversProvider::Initialize()
{
	string driversLocation = "libDrivers.so"; // use "man dlopen" to check where to put the file
	list<BaseDriver*> driversList;

	void* driverLib = dlopen( driversLocation.c_str(), RTLD_LAZY );

	if ( !driverLib )
	{
		this->GetDlError( "Cannot load library" );
		return false;
	}

	create_drivers* create = (create_drivers*) dlsym( driverLib, "CreateDrivers" );

	if ( !create )
	{
		this->GetDlError( "Cannot load symbols" );
		return false;
	}

	driversList = create();
	list<BaseDriver*>::iterator it;

	for ( it = driversList.begin(); it != driversList.end(); it++ )
	{
		this->drivers[( *it )->Name()] = ( *it );
	}

	return true;
}

