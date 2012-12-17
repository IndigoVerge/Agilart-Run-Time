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
#include "ProgramManager.h"
#include "UpcbJsonParser.h"
#include <dirent.h>

using namespace std;

ProgramManager* ProgramManager::instance = 0;

ProgramManager::ProgramManager()
{
}

ProgramManager::~ProgramManager()
{
	map<int, Process*>::iterator prgramsIt;

	for ( prgramsIt = this->runningPrograms.begin(); prgramsIt != this->runningPrograms.end(); prgramsIt++ )
	{
		if ( ( *prgramsIt ).second != NULL )
			delete ( *prgramsIt ).second;
	}

	availablePrograms.clear();
	runningPrograms.clear();
	programMappings.clear();

	delete instance;
}

ProgramManager* ProgramManager::Instance()
{
	if ( instance == NULL )
		instance = new ProgramManager();

	return instance;
}

void ProgramManager::AddProgram( string json )
{
	Program* program = UpcbJsonParser::Instance()->ParseProgram( json );

	if( program == NULL )
		return;

	this->availablePrograms[program->Id()] = json;

	//Save Program & Mapping:
	string storageFolderPath = Configuration::Instance()->GetStringParam( PROGRAMS_STORAGE_DIR );

	struct stat st;
	if ( stat( storageFolderPath.c_str(), &st ) != 0 )
		mkdir( storageFolderPath.c_str(), 0777 );

	string programSavePath = storageFolderPath + "/" + program->Name() + ".xml";
	DataSerializationManager::Save( json, programSavePath.c_str() );

	// delete program;
}

void ProgramManager::AddProgramMapping( string json )
{
	ProgramMapping* mapping = UpcbJsonParser::Instance()->ParseProgramMapping( json );
	this->programMappings[ mapping->ProgramId() ] = json;

	//delete mapping;

	//Save Program & Mapping:
	string storageFolderPath = Configuration::Instance()->GetStringParam( PROGRAMS_STORAGE_DIR );

	struct stat st;
	if ( stat( storageFolderPath.c_str(), &st ) != 0 )
		mkdir( storageFolderPath.c_str(), 0777 );

	string mappingSavePath = storageFolderPath + "/" + mapping->ProgramName() + "_mapping.xml";
	DataSerializationManager::Save( json, mappingSavePath.c_str() );
}

int ProgramManager::StartProgram( int programId )
{
	if ( this->runningPrograms[programId] != NULL )
	{
		EventLogger::Instance()->WriteVerbose(
				"Program with id '%d' is already started.", programId );
		return ERROR_PRSTARTED_ID;
	}

	string upcbDescriptor = this->availablePrograms[ programId ];
	string upcbProgramMapping = this->programMappings[ programId ];

	if ( upcbDescriptor == "" )
	{
		EventLogger::Instance()->WriteVerbose(
				"The program with id - '%d' is not available.", programId );
		return ERROR_PRNOTFOUND_ID;
	}
	if ( upcbProgramMapping == "" )
	{
		EventLogger::Instance()->WriteVerbose(
				"The program mappings for program with id %d are not available.",
				programId );
		return ERROR_NOMAPPING_ID;
	}

	Program* program = UpcbJsonParser::Instance()->ParseProgram( upcbDescriptor );

	if ( program == NULL )
	{
		EventLogger::Instance()->WriteVerbose(
				"The program with id - %d could not be created from the given description.",
				programId );
		return ERROR_LOADINGERROR_ID;
	}

	// Initialize program mappings:
	ProgramMapping* mapping = UpcbJsonParser::Instance()->ParseProgramMapping( upcbProgramMapping );
	MappingManager::Instance()->InitializeProgram( program, mapping->PinoutMappings() );
	Process* newProcess = new Process();
	newProcess->RegisterErrorHandler( this );
	newProcess->Start( program );

	this->runningPrograms[programId] = newProcess;

	EventLogger::Instance()->WriteVerbose(
		"The program with id - %d was successfully started.",
		programId );
	return 0;
}

int ProgramManager::StopProgram( int programId )
{
	Process* process = this->runningPrograms[programId];
	if ( process == NULL )
	{
		EventLogger::Instance()->WriteVerbose( "Program with id %d is not started!", programId );
		return ERROR_PRNOTSTARTED_ID;
	}

	Program* program = process->GetProgram();
	if ( program == NULL )
	{
		EventLogger::Instance()->WriteVerbose( "Program with id %d is not started!", programId );
		return ERROR_PRNOTSTARTED_ID;
	}

	process->Stop();

	// to do !!!
	MappingManager::Instance()->FinalizeProgram( program );

	// delete process & program;
	// delete this->runningPrograms[program->Id()];
	this->runningPrograms[program->Id()] = NULL;
	this->runningPrograms.erase( program->Id() );

	return 0;
}

void ProgramManager::DeleteProgram( int programId )
{
	Process* process = this->runningPrograms[programId];
	if ( process != NULL )
	{
		this->StopProgram( programId );
	}

	string programDescription = this->availablePrograms[programId];

	if( programDescription == "" )
		return;

	Program* program = UpcbJsonParser::Instance()->ParseProgram( programDescription );

	if ( program == NULL )
		return;


	//Delete Program & Mapping:
	string storageFolderPath = Configuration::Instance()->GetStringParam( PROGRAMS_STORAGE_DIR );

	struct stat st;
	if ( stat( storageFolderPath.c_str(), &st ) != 0 )
		mkdir( storageFolderPath.c_str(), 0777 );

	string programSavePath = storageFolderPath + "/" + program->Name() + ".xml";
	string mappingSavePath = storageFolderPath + "/" + program->Name() + "_mapping.xml";

	program = NULL;
	this->availablePrograms.erase( programId );
	this->programMappings.erase( programId );

	DataSerializationManager::Delete( mappingSavePath.c_str() );
	DataSerializationManager::Delete( programSavePath.c_str() );
}

Program* ProgramManager::GetRunningProgram( int programId )
{
	Process* process = this->runningPrograms[programId];

	if(process == NULL)
		return NULL;

	return process->GetProgram();
}

void ProgramManager::LoadProgram( string programName, string mappingName )
{
	string storageFolderPath = Configuration::Instance()->GetStringParam(
			PROGRAMS_STORAGE_DIR );
	string programSavePath = storageFolderPath + "/" + programName;
	string mappingSavePath = storageFolderPath + "/" + mappingName;

	string programDefinition = DataSerializationManager::Load( programSavePath.c_str() );
	string mappingDefinition = DataSerializationManager::Load( mappingSavePath.c_str() );

	Program* program = UpcbJsonParser::Instance()->ParseProgram( programDefinition );
	if( program == NULL )
		return;

	this->availablePrograms[program->Id()] = programDefinition;
	this->programMappings[program->Id()] = mappingDefinition;

	//delete program;
}

void ProgramManager::InitializePrograms()
{
	list<string> programFiles;
	list<string> mappingFiles;

	string fileName;
	string mappingFileName;
	string mapping = "_mapping";
	string extension = ".xml";
	size_t pos;

	DIR *dir;
	string storageFolderPath = Configuration::Instance()->GetStringParam(
			PROGRAMS_STORAGE_DIR );

	struct dirent *ent;
	dir = opendir( storageFolderPath.c_str() );

	if ( dir != NULL )
	{
		/* print all the files and directories within directory */
		while ( ( ent = readdir( dir ) ) != NULL )
		{
			fileName = ent->d_name;
			if ( fileName == "." || fileName == ".." )
			{
				continue;
			}
			else if ( std::string::npos == fileName.find( mapping ) ) // _mapping is not found
			{
				programFiles.push_back( fileName );
			}
			else
			{
				mappingFiles.push_back( fileName );
			}
		}

		closedir( dir );
	}
	else
	{
		/* could not open directory */
		EventLogger::Instance()->WriteVerbose(
				"Could not open the UPCB Storage Directory: %s \n",
				storageFolderPath.c_str() );
		return;
	}

	list<string>::iterator i;
	for ( i = programFiles.begin(); i != programFiles.end(); i++ )
	{
		fileName = ( *i );
		pos = fileName.find( extension );
		mappingFileName = fileName.substr( 0, pos ) + mapping + extension;

		this->LoadProgram( fileName, mappingFileName );
	}

	string autoStartProgramIds = Configuration::Instance()->GetStringParam( CONFIG_AUTOSTARTIDS );

	size_t startPos = 0;
	size_t commaPos = autoStartProgramIds.find( ',', startPos );

	string current;
	int programId;

	while( commaPos != string::npos )
	{
		current = autoStartProgramIds.substr(startPos, commaPos - startPos );
		if( current != "" )
		{
			programId = atoi( current.c_str() );
			this->autoStartedProgramsIds.push_back( programId );
			this->StartProgram( programId );
		}

		startPos = commaPos + 1;
		commaPos = autoStartProgramIds.find( ',', startPos );
	}

	current = autoStartProgramIds.substr(startPos, autoStartProgramIds.size() - startPos );
	if( current != "" )
	{
		programId = atoi( current.c_str() );
		this->autoStartedProgramsIds.push_back( programId );
		this->StartProgram( programId );
	}

}

list<string> ProgramManager::GetAllPrograms()
{
	map<int, string>::iterator i;

	list<string> programs;

	bool isRunning = false;
	bool isAutoStart = false;
	string programJson;

	for ( i = this->availablePrograms.begin(); i != this->availablePrograms.end(); i++ )
	{
		programJson = (*i).second;

		if( programJson == "" )
			continue;

		isRunning = this->runningPrograms.find( (*i).first ) != this->runningPrograms.end();
		if(isRunning) {
			Process* process = this->runningPrograms[(*i).first ];
			if(process == NULL)
				isRunning = false;
		}

		isAutoStart = find( this->autoStartedProgramsIds.begin(), this->autoStartedProgramsIds.end(), (*i).first ) != this->autoStartedProgramsIds.end();

		programJson[programJson.size() - 1] = ',';
		programJson += "\"" + JsonParameters::ProgramAutoStart + "\":\"";

		if( isAutoStart == true ) {
			programJson += "true";
		} else {
			programJson += "false";
		}

		programJson += "\",\"" + JsonParameters::ProgramIsRunning + "\":\"";

		if( isRunning == true ) {
			programJson += "true\"}";
		} else {
			programJson += "false\"}";
		}

		programs.push_back( programJson );
	}

	return programs;
}

string ProgramManager::GetProgram( int programId )
{
	map<int, string>::iterator i;
	string program = this->availablePrograms[programId];

	return program;
}

string ProgramManager::GetMapping( int programId )
{
	map<int, string>::iterator i;
	string programMapping = this->programMappings[programId];

	return programMapping;
}

string ProgramManager::GetProcessStatus( int id, string serviceBaseAddress )
{
	Process* process = this->runningPrograms[id];
	if ( process == NULL )
	{
		EventLogger::Instance()->WriteVerbose( "Program with id %d is not started!", id );
		return "{\"Status\":\"NotAvailable\"}";
	}

	return process->GetStatus( serviceBaseAddress );
}

void ProgramManager::SaveCustomDevice( string content )
{
	string storageFolderPath = Configuration::Instance()->GetStringParam(
				CONFIG_STORAGE_DIR );

	string filePath = storageFolderPath + "/" + (string)DEVICES_DESCR_FILE;

	if( DataSerializationManager::FileExists( filePath.c_str() ) == false )
	{
		DataSerializationManager::Save( content, filePath );
		return;
	}

	string fileContent = DataSerializationManager::Load( filePath );
	fileContent += "," + content;

	DataSerializationManager::Save( fileContent, filePath );

	BaseDevice* device = UpcbJsonParser::Instance()->ParseCustomDevice( content );
	if( device != NULL )
	{
		device->SetIsCustom( true );
		DeviceManager::Instance()->AddDevice( device );
	}
}

void ProgramManager::SaveProgramAutoStartSetting( int programId, bool isAutoStarted )
{
	string autoStartProgramIds = Configuration::Instance()->GetStringParam( CONFIG_AUTOSTARTIDS );

	char buffer[32];
	sprintf( buffer, "%d", programId );
	string programIdString = (string)buffer;
	size_t index;

	if( isAutoStarted == false )
	{
		this->autoStartedProgramsIds.remove( programId );
		index = autoStartProgramIds.find( programIdString, 0 );

		// If the id of the program is already not in the autostart list > do nothing
		if(index == string::npos )
			return;

		size_t end = index + programIdString.size();
		if( end < autoStartProgramIds.size() - 1 )
			end += 1;

		autoStartProgramIds.replace(index, end, "" );
	}
	else
	{
		index = autoStartProgramIds.find( programIdString, 0 );
		// If the id of the program is already in the autostart list > do nothing
		if(index != string::npos )
			return;

		if(autoStartProgramIds != "")
			autoStartProgramIds += ",";

		autoStartProgramIds += programIdString;
		this->autoStartedProgramsIds.push_back( programId );
	}

	Configuration::Instance()->SetParam( ((string)CONFIG_AUTOSTARTIDS).c_str(), autoStartProgramIds.c_str() );
	Configuration::Instance()->SaveToConfigFile();
}

void ProgramManager::HandleError( void* sender, ErrorItem* error)
{
	Process* process = ( Process* )sender;
	if( process == NULL )
		return;

	Program* program = process->GetProgram();
	if ( program == NULL )
	{
		EventLogger::Instance()->WriteVerbose( "Program with id %d is still not started but an Error in the process with the same id is found!", program->Id() );
		return;
	}

	process->Stop();

	// to do !!!
	MappingManager::Instance()->FinalizeProgram( program );
	// delete process & program;
	// delete this->runningPrograms[program->Id()];
	this->runningPrograms[program->Id()] = NULL;
	this->runningPrograms.erase( program->Id() );
}
