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

#ifndef PROGRAMMANAGER_H_
#define PROGRAMMANAGER_H_

#include <iostream>
#include <map>
#include <sys/stat.h>
#include <sys/types.h>

#include "Program.h"
#include "Mappings/PinoutMapping.h"
#include "UPCBDescriptors.h"
#include "BaseDevice.h"
#include "ConnectionPoint.h"
#include "Logging/EventLogger.h"
#include "Mappings/MappingManager.h"
#include "Command.h"
#include "Wire.h"
#include "Devices/DeviceManager.h"
#include "DataSerializationManager.h"
#include "Configuration.h"
#include "Consts.h"
#include "Process.h"
#include "IErrorHandler.h"

using namespace std;

class ProgramManager: public IErrorHandler
{
	private:
		static ProgramManager* instance;

		//map<string, ProgramDescriptor*> availablePrograms;
		map<int, string> availablePrograms; //-- <name, JSON Description>
		map<int, Process*> runningPrograms;
		//map<string, ProgramMappingDescriptor*> programMapping;
		map<int, string> programMappings;

		list<int> autoStartedProgramsIds;

		void LoadProgram( string, string );

	protected:
		ProgramManager();

	public:
		virtual ~ProgramManager();
		static ProgramManager* Instance();

		void InitializePrograms();

		void AddProgram( string );
		void AddProgramMapping( string );

		int StartProgram( int );
		int StopProgram( int );
		void DeleteProgram( int );

		Program* GetRunningProgram( int );

		list<string> GetAllPrograms();
		string GetProgram( int programId );
		string GetMapping( int programId );

		string GetProcessStatus( int id, string serviceBaseAddress );

		void SaveCustomDevice( string content );
		void SaveProgramAutoStartSetting( int programId, bool isAutoStarted );

		virtual void HandleError( void* sender, ErrorItem* error);
};

#endif /* PROGRAMMANAGER_H_ */
