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
#include <string>
#include <map>
#include "Program.h"
#include "Enums.h"
#include "BaseDevice.h"
#include "ConnectionPoint.h"
#include "Logging/EventLogger.h"
#include "Common/Mappings/PinoutMapping.h"
#include "Common/Mappings/MappingManager.h"
#include "Command.h"
#include "Wire.h"
#include "Common/ProgramManager.h"

#include "gSOAP/WebServer.h"
#include "gSOAP/SoapServiceImplementation.h"

#include "Common/Drivers/PluggableDriversProvider.h"
#include "Common/Devices/PluggableDevicesProvider.h"
#include "Common/Devices/DeviceManager.h"
#include "Common/Devices/BaseDevicesProvider.h"
#include "Common/Drivers/DriversManager.h"
#include "Common/Drivers/BaseDriversProvider.h"

#include "DeviceParameter.h"

#include "errno.h"
#define CONFIG_FILE_PATH "/etc/upcb.conf"

using namespace std;

int main( int argc, char* argv[] )
{
	if( argc != 3 )
	{
		cout << "Usage: <ip> <port>!\n";
		return 0;
	}

	int port = atoi( argv[2] );
	char* ip = argv[1];

	// Initialize the path to the config file:
	Configuration::Initialize( CONFIG_FILE_PATH, NULL );

	BaseDevicesProvider* deviceProvider = new PluggableDevicesProvider();
	BaseDriversProvider* driversProvider = new PluggableDriversProvider();
	driversProvider->Initialize();

	DeviceManager::Instance()->RegisterProvider( deviceProvider );
	DriversManager::Instance()->RegisterProvider( driversProvider );

	deviceProvider->Initialize();
	ProgramManager::Instance()->InitializePrograms();

	WebServer::Instance()->Start( port, ip );

	string x;
	cout << "Press any key to stop the Agilart run-time executable.\n";
	cin >> x;

	WebServer::Instance()->Stop();

	return 0;
}
