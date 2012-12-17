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

#include "DriversManager.h"
#include "Logging/EventLogger.h"
#include "Consts.h"
#include <iostream>

using namespace std;

DriversManager* DriversManager::instance = 0;

DriversManager* DriversManager::Instance()
{
	if ( instance == NULL )
		instance = new DriversManager();

	return instance;
}

DriversManager::DriversManager()
{
}

DriversManager::~DriversManager()
{
	// TODO Auto-generated destructor stub
}

BaseDriver* DriversManager::GetDriver( const char *name )
{
	list<BaseDriversProvider*>::iterator iterator;
	bool supportsDriver;

	for ( iterator = this->driverProviders.begin(); iterator != this->driverProviders.end(); iterator++ )
	{
			supportsDriver = ( *iterator )->SupportsDriver( name );

			if ( supportsDriver == true )
				return ( *iterator )->GetDriver( name );
	}

	EventLogger::Instance()->WriteVerbose( "Driver not found - %s!", name );
	return NULL;
}

string DriversManager::GetDriversJsonDescription()
{
	list<BaseDriver*>::iterator it;
	list<BaseDriversProvider*>::iterator providersIt;
	list<BaseDriver*> drivers;
	list<BaseDriver*> allDrivers;

	string jsonDescr = "[";
	int i = 0;

	for ( providersIt = this->driverProviders.begin(); providersIt
			!= this->driverProviders.end(); providersIt++ )
	{
		drivers = ( *providersIt )->Drivers();

		for ( it = drivers.begin(); it != drivers.end(); it++ )
			allDrivers.push_back( *it );
	}

	for ( it = allDrivers.begin(); it != allDrivers.end(); it++ )
	{
		i++;
		if ( i != allDrivers.size() )
		    jsonDescr += (*it)->ToJson() + ", ";
		else
			jsonDescr += (*it)->ToJson();
	}

    jsonDescr += "]";

	return jsonDescr;
}

void DriversManager::UnregisterProvider( BaseDriversProvider* provider )
{
	this->driverProviders.remove( provider );
}

void DriversManager::RegisterProvider( BaseDriversProvider* provider )
{
	this->driverProviders.push_back( provider );
}
