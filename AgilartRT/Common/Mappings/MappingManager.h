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

#ifndef MAPPINGMANAGER_H_
#define MAPPINGMANAGER_H_

#include <list>
#include <map>

#include "Program.h"
#include "PinoutMapping.h"
#include "BaseDriver.h"
#include "../Drivers/BaseDriversProvider.h"

using namespace std;

class MappingManager
{
	private:

		friend void DriverPinoutStatusChanged( void* sender, Event senderEvent );
		friend void DevicePinoutExecuteCommand( Pinout* sender, Command senderCommand );

		static MappingManager* instance;
		MappingManager();

		map<Pinout*, BaseDriver*> pinoutDrivers;
		map<BaseDriver*, Pinout*> driverPinouts;

		map<Pinout*, PinoutMapping*> pinoutMappings;
		map<Pinout*, BaseDevice*> pinoutToDevice;
		list<Pinout*> pinouts;

		/*
		 * Attached to the ExecuteCommandEvent of the Pinouts
		 */
		void DeviceExecuteCommand( Pinout*, Command );

		/*
		 * Attached to the StatusChangedEvent of the Driver
		 */
		void PinoutStatusChanged( void*, Event );

		/*
		 * Retrieves the mapping with pinoutId = the first param and
		 * deviceId = the second param from the mappings in the list
		 */
		PinoutMapping* GetMapping( int, int, list<PinoutMapping*>* );

	public:
		virtual ~MappingManager();
		static MappingManager* Instance();

		void InitializeProgram( Program*, list<PinoutMapping*>* );
		void FinalizeProgram( Program* );
};

#endif /* MAPPINGMANAGER_H_ */
