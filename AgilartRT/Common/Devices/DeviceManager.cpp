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

#include "DeviceManager.h"
#include "Logging/EventLogger.h"
#include "Consts.h"
#include <iostream>

using namespace std;

DeviceManager* DeviceManager::instance = 0;

DeviceManager* DeviceManager::Instance()
{
	if ( instance == NULL )
		instance = new DeviceManager();

	return instance;
}

DeviceManager::DeviceManager()
{
	// TODO Auto-generated constructor stub

}

DeviceManager::~DeviceManager()
{
	// TODO Auto-generated destructor stub
}

BaseDevice* DeviceManager::CreateFromDescriptor( DeviceDescriptor devDescriptor )
{
	string deviceType;
	int deviceId;

	DescriptorsUtil::GetStringProperty( devDescriptor.Properties, "Name",
			&deviceType );
	DescriptorsUtil::GetIntProperty( devDescriptor.Properties, "Id", &deviceId );

	BaseDevice* device = this->GetDevice( deviceType.c_str() );
	if( device == NULL )
	{
		EventLogger::Instance()->WriteVerbose("Device of type %s is not available!",deviceType.c_str() );
		return NULL;
	}

	device->SetId( deviceId );
	device->InitializeFromDescriptor( devDescriptor.Properties );

	return device;
}

BaseDevice* DeviceManager::GetDevice( const char *name )
{
	list<BaseDevicesProvider*>::iterator iterator;
	bool supportsDevice;

	for ( iterator = this->deviceProviders.begin(); iterator
			!= this->deviceProviders.end(); iterator++ )
	{
		supportsDevice = ( *iterator )->SupportsDevice( name );

		if ( supportsDevice == true )
		{
			BaseDevice* resultDevice = ( *iterator )->GetDevice( name );
			return resultDevice;
		}
	}

	EventLogger::Instance()->WriteVerbose( "The Device - %s is not available", name );
	return NULL;
}


void DeviceManager::AddDevice( BaseDevice* device )
{
	list<BaseDevicesProvider*>::iterator iterator;

	for ( iterator = this->deviceProviders.begin(); iterator
			!= this->deviceProviders.end(); iterator++ )
	{
		if( (*iterator)->AddDevice(device) == true )
			break;
	}
}

void DeviceManager::UnregisterProvider( BaseDevicesProvider* provider )
{
	this->deviceProviders.remove( provider );
}

void DeviceManager::RegisterProvider( BaseDevicesProvider* provider )
{
	this->deviceProviders.push_back( provider );
}

list<DeviceDescriptor>* DeviceManager::GetDeviceDescriptors()
{
	list<DeviceDescriptor>* deviceDescriptors = new list<DeviceDescriptor> ;

	list<BaseDevice*>::iterator i;
	list<BaseDevicesProvider*>::iterator providersIt;
	list<BaseDevice*> devices;

	for ( providersIt = this->deviceProviders.begin(); providersIt
			!= this->deviceProviders.end(); providersIt++ )
	{
		devices = ( *providersIt )->Devices();

		for ( i = devices.begin(); i != devices.end(); i++ )
			deviceDescriptors->push_back( ( *i )->GetDescriptor() );
	}

	return deviceDescriptors;
}

string DeviceManager::GetDevicesJsonDescription()
{
	list<BaseDevicesProvider*>::iterator it;
	list<BaseDevice*> allDevices;
	list<BaseDevice*> devices;
	list<BaseDevice*>::iterator devIt;

	string jsonDescr = "[";
	int i = 0;

	for( it = this->deviceProviders.begin(); it != this->deviceProviders.end(); it++ )
	{
		devices = (*it)->Devices();

		for( devIt = devices.begin(); devIt != devices.end(); devIt++ )
			allDevices.push_back(*devIt);
	}

	for ( devIt = allDevices.begin(); devIt != allDevices.end(); devIt++ )
	{
		i++;
		if ( i != allDevices.size() )
		    jsonDescr += (*devIt)->ToJson() + ", ";
		else
			jsonDescr += (*devIt)->ToJson();
	}

    jsonDescr += "]";

	return jsonDescr;

}
