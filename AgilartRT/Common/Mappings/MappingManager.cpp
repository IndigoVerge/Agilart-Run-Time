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
#include "MappingManager.h"
#include "../Drivers/DriversManager.h"
#include "Logging/EventLogger.h"
#include "CommandParameter.h"
#include "CommandMapping.h"
#include "CommandParameterMapping.h"
#include <iostream>
#include <sstream>

using namespace std;

MappingManager* MappingManager::instance = 0;

MappingManager::~MappingManager()
{
}

MappingManager::MappingManager()
{
}

MappingManager* MappingManager::Instance()
{
	if ( instance == NULL )
		instance = new MappingManager();

	return instance;
}

void MappingManager::DeviceExecuteCommand( Pinout* sender, Command senderCommand )
{
	PinoutMapping* pinoutMapping = this->pinoutMappings[sender];
	CommandMapping* commandMapping = pinoutMapping->GetCommandMapping( senderCommand.Name() );

	// get pinout driver
	BaseDriver* driver = this->pinoutDrivers[sender];

	if( commandMapping == NULL )
	{
		// Command not found in the mapping (the driver may not include that command)
		// send the command without mapping
		driver->ExecuteCommand( senderCommand );
		return;
	}

	Command command( commandMapping->Target() ); //remapped according to the pinout mapping

	list<CommandParameter*>::iterator it;
	list<CommandParameter*>* commandParams = senderCommand.Parameters();

	for( it = commandParams->begin(); it != commandParams->end(); it++ )
	{
		CommandParameterMapping* paramMapping = commandMapping->GetCommapndParameterMapping((*it)->Key);
		CommandParameter* param = new CommandParameter(paramMapping->Target(), (*it)->Value, (*it)->Type);

		command.AddParameter( param );
	}

	list<CommandParameterMapping*>::iterator i;
	for(i = commandMapping->Parameters()->begin(); i != commandMapping->Parameters()->end(); i++)
	{
		if( (*i)->DefaultValue() == NULL )
			continue;

		CommandParameter* param = new CommandParameter( (*i)->Target().c_str(), (*i)->DefaultValue(), (*i)->DefaultType().c_str() );
		command.AddParameter( param );
	}

	driver->ExecuteCommand( command );
}

// execute a method on the pinout
void MappingManager::PinoutStatusChanged( void* sender, Event senderEvent )
{
	BaseDriver* driver = (BaseDriver*)sender;
	Pinout* pinout = this->driverPinouts[driver];

	PinoutMapping* pinoutMapping = this->pinoutMappings[pinout];
	EventMapping* eventMapping = pinoutMapping->GetEventMapping( senderEvent.Name() );

	//Event targetEvent( eventMapping->Target() );
	Event targetEvent( eventMapping->Source() );

	list<EventParameter>::iterator it;
	for( it = senderEvent.Parameters()->begin(); it != senderEvent.Parameters()->end(); it++ )
	{
		EventParameterMapping* paramMapping = eventMapping->GetEventParameterMapping( (*it).Key );
		EventParameter param( paramMapping->Target().c_str(), (*it).Value, (*it).Type );

		targetEvent.AddParameter( param );
	}

	BaseDevice* targetDevice = this->pinoutToDevice[pinout];
	pinout->EventTriggered( targetDevice, *(targetEvent.Parameters()) );
}

PinoutMapping* MappingManager::GetMapping( int pinoutId, int deviceId, list<
		PinoutMapping*>* mappings )
{
	list<PinoutMapping*>::iterator i;

	for ( i = mappings->begin(); i != mappings->end(); i++ )
	{
		if ( ( *i )->DeviceId() == deviceId && ( *i )->PinoutId() == pinoutId )
			return ( *i );
	}

	return NULL;
}

void DriverPinoutStatusChanged( void* sender, Event senderEvent )
{
	MappingManager::Instance()->PinoutStatusChanged( sender, senderEvent );
}

void DevicePinoutExecuteCommand(Pinout* sender, Command senderCommand)
{
	MappingManager::Instance()->DeviceExecuteCommand( sender, senderCommand  );
}

void MappingManager::InitializeProgram( Program* program,
		list<PinoutMapping*>* mappings )
{
	EventLogger::Instance()->WriteVerbose(
			"MappingManager - InitializeProgram with id - %d", program->Id() );

	list<BaseDevice*>* devices = program->Devices();
	list<BaseDevice*>::iterator deviceIt;
	list<Pinout*>::iterator pinoutIt;
	list<Pinout*>* pinouts;
	int pinoutId;
	char numstr[21];

	for ( deviceIt = devices->begin(); deviceIt != devices->end(); deviceIt++ )
	{
		BaseDevice* device = ( *deviceIt );
		pinouts = device->Pinouts();

		sprintf( numstr, "%d", device->Id() );
		string deviceId = numstr;
		string logMessage;

		for ( pinoutIt = pinouts->begin(); pinoutIt != pinouts->end(); pinoutIt++ )
		{
			PinoutMapping* pinMapping = this->GetMapping( ( *pinoutIt )->Id(),
					device->Id(), mappings );

			sprintf( numstr, "%d", ( *pinoutIt )->Id() );
			string pinId = numstr;

			if ( pinMapping == NULL )
			{
				EventLogger::Instance()->WriteVerbose( "No driver found for the device with id - %d!", device->Id() );
				continue;
			}

			pinoutId = pinMapping->PinoutId();

			BaseDriver* driver = DriversManager::Instance()->GetDriver(
					pinMapping->DriverType().c_str() );

			this->pinoutDrivers[( *pinoutIt )] = driver;
			this->driverPinouts[driver] = *pinoutIt;
			this->pinoutMappings[( *pinoutIt )] = pinMapping;
			this->pinoutToDevice[( *pinoutIt )] = device;

			this->pinouts.push_back( *pinoutIt );

			driver->StatusChangedEvent = &DriverPinoutStatusChanged;
			( *pinoutIt )->ExecuteCommandEvent = &DevicePinoutExecuteCommand;
		}
	}

	EventLogger::Instance()->WriteInformation( "Mapping handlers attached." );
}


void MappingManager::FinalizeProgram( Program* program )
{
	list<BaseDevice*>::iterator deviceIt;
	list<Pinout*>::iterator pinoutIt;
	list<Pinout*>* pinouts;

	for ( deviceIt = program->Devices()->begin(); deviceIt != program->Devices()->end(); deviceIt++ )
	{
		BaseDevice* device = ( *deviceIt );
		pinouts = device->Pinouts();

		for ( pinoutIt = pinouts->begin(); pinoutIt != pinouts->end(); pinoutIt++ )
		{
			//map<Pinout*, BaseDriver*> pinoutDrivers;
			//map<BaseDriver*, Pinout*> driverPinouts;

			//map<Pinout*, PinoutMapping*> pinoutMappings;
			//map<Pinout*, BaseDevice*> pinoutToDevice;

			BaseDriver* driver = this->pinoutDrivers[*pinoutIt];
			this->driverPinouts.erase( driver );

			driver = NULL;
			this->pinoutDrivers.erase( *pinoutIt );
			this->pinoutMappings.erase( *pinoutIt );
			this->pinoutToDevice.erase( *pinoutIt );
		}
	}
}
