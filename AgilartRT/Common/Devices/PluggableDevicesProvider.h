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

#ifndef PLUGGABLEDEVICESPROVIDER_H_
#define PLUGGABLEDEVICESPROVIDER_H_

#include "BaseDevicesProvider.h"
#include "BaseDevice.h"
#include "Logging/EventLogger.h"

#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <map>
#include <list>

using namespace std;

class PluggableDevicesProvider : public BaseDevicesProvider
{
	private:
		char *plugindir;
		map<const char*, BaseDevice*> devices;

		void AddDevice( const char*, BaseDevice* );
		int GetDlError(const char *);
		bool LoadSharedLibrary( string );

		void InitializeCustomDevices();
		bool InitializeUploadedDevice();

	public:
		PluggableDevicesProvider();
		virtual ~PluggableDevicesProvider();

		virtual list<BaseDevice*> Devices();
		virtual bool SupportsDevice( const char * );

		virtual BaseDevice* GetDevice( const char * );
		virtual bool AddDevice( BaseDevice* );

		bool Initialize();
};

#endif /* PLUGGABLEDEVICESPROVIDER_H_ */
