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
#include "PluggableDevicesProvider.h"

#include <stdlib.h>
#include "Configuration.h"
#include "Consts.h"
#include "DataSerializationManager.h"
#include "../UpcbJsonParser.h"

using namespace std;

PluggableDevicesProvider::PluggableDevicesProvider()
{
}

PluggableDevicesProvider::~PluggableDevicesProvider()
{
	//this->devices.Clear();
}

int PluggableDevicesProvider::GetDlError( const char *message )
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

void PluggableDevicesProvider::InitializeCustomDevices()
{
	string storageFolderPath = Configuration::Instance()->GetStringParam(
			CONFIG_STORAGE_DIR );

	string filePath = storageFolderPath + "/" + (string)DEVICES_DESCR_FILE;
	string json = DataSerializationManager::Load(filePath);

	size_t deviceIdIndex;
	size_t nameIndex;
	size_t baseTypeIndex;
	size_t propertiesIndex;
	size_t startFrom = 0;
	size_t start;
	size_t end;
	size_t endIndex;

	string deviceJson;

	while(true)
	{
		deviceIdIndex = json.find( JsonParameters::Id, startFrom );
		if ( deviceIdIndex == string::npos)
			break;

		nameIndex = json.find( JsonParameters::CustomDeviceName, startFrom );
		baseTypeIndex = json.find( JsonParameters::CustomDeviceBaseType, startFrom );
		propertiesIndex = json.find( JsonParameters::CustomDeviceAddOnProps, startFrom );

		endIndex = json.find( "]", propertiesIndex );
		startFrom = endIndex + 1;

		start = deviceIdIndex;
		end = endIndex;
		deviceJson = json.substr( start, end - start );

		BaseDevice* customDevice = UpcbJsonParser::Instance()->ParseCustomDevice( deviceJson );
		if( customDevice != NULL )
		{
			customDevice->SetIsCustom( true );
			this->devices[customDevice->Name()] = customDevice;
		}
	}
}

list<BaseDevice*> PluggableDevicesProvider::Devices()
{
	list<BaseDevice*> devicesList;
	map<const char*, BaseDevice*>::iterator it;

	for ( it = this->devices.begin(); it != this->devices.end(); it++ )
		devicesList.push_back( it->second );

	return devicesList;
}

bool PluggableDevicesProvider::SupportsDevice( const char * type )
{
	map<const char*, BaseDevice*>::iterator it;

	for( it = this->devices.begin(); it != this->devices.end(); it++ )
	{
		if( !strcmp((it)->first, type) )
			return true;
	}

	return false;
}

BaseDevice* PluggableDevicesProvider::GetDevice( const char * type )
{
	map<const char*, BaseDevice*>::iterator it;

	for( it = this->devices.begin(); it != this->devices.end(); it++ )
	{
		if( !strcmp((it)->first, type) )
			return ((it)->second)->CreateInstance();
	}

	return NULL;
}


bool PluggableDevicesProvider::AddDevice( BaseDevice* device )
{
	if( this->SupportsDevice( device->Name()) == true )
		throw( "The device already exists" );

	this->devices[device->Name()] = device;
	return true;
}

bool PluggableDevicesProvider::LoadSharedLibrary( string path )
{
	list<BaseDevice*> devicesList;
	void* deviceLib = dlopen( path.c_str(), RTLD_LAZY );

	if ( !deviceLib )
	{
		this->GetDlError( "Cannot load library" );
		return false;
	}

	create_devices* create = (create_devices*) dlsym( deviceLib, DEVICES_ENTYPOINT_SMBNAME );

	if ( !create )
	{
		this->GetDlError( "Cannot load symbols" );
		return false;
	}

	devicesList = create();
	list<BaseDevice*>::iterator it;

	for ( it = devicesList.begin(); it != devicesList.end(); it++ )
	{
		this->AddDevice( ( *it )->Name(), *it );
	}

	return true;
}

void PluggableDevicesProvider::AddDevice( const char* key, BaseDevice* device )
{
	map<const char*, BaseDevice*>::iterator it;

	for( it = this->devices.begin(); it != this->devices.end(); it++ )
	{
		if( !strcmp((it)->first, key) )
		{
			EventLogger::Instance()->WriteVerbose(
				"Adding the %s device didn't succeed because a device with the same name already exists in the Pluggable Drivers Provider.", key );
			return;
		}
	}

	this->devices[key] = device;
}

bool PluggableDevicesProvider::InitializeUploadedDevice()
{
	string storageDir = Configuration::Instance()->GetStringParam(CONFIG_STORAGE_DIR);
	string devicesDir = storageDir + "/" + CONFIG_Devices_DIR;
	string libraryName;

	DIR *dir;
	struct dirent *ent;
	dir = opendir ( devicesDir.c_str() );

	string homeDir = ".";
	string parentDir = "..";

	if (dir == NULL)
	{
		EventLogger::Instance()->WriteVerbose( "Could not open directory: %s.", devicesDir.c_str() );
		perror ("Could not open directory." );
		return false;
	}

	/* go through the files and directories within the storage directory */
	while ((ent = readdir (dir)) != NULL)
	{
		if( !strcmp(ent->d_name, homeDir.c_str()) || !strcmp(ent->d_name, parentDir.c_str()) )
			continue;

		libraryName = devicesDir + "/" + (string)(ent->d_name);
		this->LoadSharedLibrary( libraryName );
	}

	closedir (dir);

	return true;
}

bool PluggableDevicesProvider::Initialize()
{
	string devicesLocation = "libDevices.so";
	list<BaseDevice*> devicesList;

	void* devicesLib = dlopen( devicesLocation.c_str(), RTLD_LAZY );

	if ( !devicesLib )
	{
		this->GetDlError( "Cannot load library" );
		return false;
	}

	create_devices* create = (create_devices*) dlsym( devicesLib,
			DEVICES_ENTYPOINT_SMBNAME );

	if ( !create )
	{
		this->GetDlError( "Cannot load symbols" );
		return false;
	}

	devicesList = create();
	list<BaseDevice*>::iterator it;

	for ( it = devicesList.begin(); it != devicesList.end(); it++ )
	{
		this->devices[( *it )->Name()] = ( *it );
	}

	this->InitializeCustomDevices();
	this->InitializeUploadedDevice();

	return true;
}

