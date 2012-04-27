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

#include "Process.h"

Process::Process()
{
	this->errorDescription = NULL;
}

Process::Process( int id )
{
	this->id = id;
}

Process::~Process()
{
	if(this->program != NULL)
		delete this->program;
}

void Process::Start( Program* program )
{
	this->program = program;
	this->errorDescription = NULL;

	if(this->program != NULL)
		this->id = this->program->Id();

	// Initialize program initial state:
	list<BaseDevice*>* devices = program->Devices();
	list<BaseDevice*>::iterator deviceIt;

	for ( deviceIt = devices->begin(); deviceIt != devices->end(); deviceIt++ )
	{
		(*deviceIt)->RegisterErrorHandler( this );

		list<ConnectionPoint*> cps = (*deviceIt)->ConnectionPoints();

		if( cps.size() == 1 )
		{
			ConnectionPoint* startingCp = cps.front();
			if( startingCp->Direction() == DIRECTION_OUT )
			{
				(*deviceIt)->InputPinValueChanged( (void*)startingCp );
			}
		}
	}

	this->state = RUNNING;
}

void Process::Stop()
{
	list<BaseDevice*>* devices = this->program->Devices();
	list<BaseDevice*>::iterator deviceIt;

	for ( deviceIt = devices->begin(); deviceIt != devices->end(); deviceIt++ )
		( *deviceIt )->ReleaseResources();

	this->state = NOTRUNNING;
}

string Process::GetStatus( string serverBaseAddress )
{
	string jsonDescr;
	string deviceStatus;
	string propertyName = "Value";
	DeviceParameter valueParam;
	char deviceId[16];
	char connPointId[16];
	string status = "Not set";
	char buffer[32];
	sprintf( buffer, "%d", this->id );

	string processId = (string) buffer;

	string processResource = serverBaseAddress + "/Processes/" + processId;
	string projectResource = serverBaseAddress + "/Projects/" + processId;
	string operation = GetProgramStatusOperation;
	jsonDescr = "{\"ResourceId\":\"" + processResource + "\",\"ProjectResourceId\":\"" + projectResource + "\",\"Devices\":[";
	string deviceUrl;
	string propertyUrl;

	list<BaseDevice*>::iterator it;
	list<ConnectionPoint*>::iterator cpIt;
	list<ConnectionPoint*> connectionPoints;

	BaseDevice* device;
	void* value;
	string stringValue;
	int cpCount;

	for( it = program->Devices()->begin(); it != program->Devices()->end(); it++ )
	{
		device = *it;
		valueParam = ( *it )->GetParameter( propertyName );

		if ( valueParam.IsInitialized() == true )
		{
			status = valueParam.StringValue();
		}
		else
		{
			continue;
		}

		sprintf( deviceId, "%d", (*it)->Id() );
		// sprintf( status, "%d", statusValue );

		deviceUrl = processResource + "/Devices/" + (string)deviceId;
		deviceStatus = "{\"ResourceId\":\"" + deviceUrl + "\",\"Properties\":[{\"ResourceId\":\"" + deviceUrl + "/Properties/" + propertyName + "\",\"devkey\":\"" +
				propertyName + "\",\"devtype\":\"int\",\"devvalue\":\"" +  status + "\"}],";
		jsonDescr += deviceStatus;

		jsonDescr += "\"" + JsonParameters::DeviceConnectionPoints + "\":[";

		connectionPoints = device->ConnectionPoints();
		cpCount = 0;

		for( cpIt = connectionPoints.begin(); cpIt != connectionPoints.end(); cpIt++ )
		{
			cpCount++;
			sprintf( connPointId, "%d", (*cpIt)->Id() );
			value = (*cpIt)->Value();
			stringValue = Convert::ToString( value, (*cpIt)->DataType() );

			jsonDescr += "{\"" + JsonParameters::Id + "\":" + (string)connPointId  + ",\"" + JsonParameters::ConnectionPointValue + "\":\"" + stringValue + "\"}";
			if(cpCount != connectionPoints.size())
				jsonDescr += ",";
		}
		jsonDescr += "]},";
	}

	jsonDescr = jsonDescr.substr( 0, jsonDescr.length() - 1 ); // remove the last comma

	status = ProcessStateRunning;
	if( this->state == ERROR )
		status = ProcessStateError;

	string error = "";
	if( this->errorDescription != NULL )
		error = EventLogger::Instance()->GetErrorMessage( this->errorDescription->ErrorId() );

	jsonDescr += "],\"Status\":\"" + status + "\",\"ErrorMessage\":\"" + error + "\"}";

	return jsonDescr;
}

void Process::HandleError( void* sender, ErrorItem* error)
{
	if( sender == NULL || error == NULL )
	{
		EventLogger::Instance()->WriteVerbose(
				"The HandleError handle of the Process with id = %d could not be executed. The error sender and the error item should not be NULL!",
				this->id );
		return;
	}

	this->state = ERROR;
	this->errorDescription = error;
	// BaseDevice* device  = (BaseDevice*) sender;

	if( this->errorsHandler != NULL )
	{
		(this->errorsHandler->*ErrorHandler)( this, error );
	}
}

Program* Process::GetProgram()
{
	return this->program;
}

ProcessState Process::GetState()
{
	return this->state;
}

int Process::Id()
{
	return this->id;
}

void Process::RegisterErrorHandler( IErrorHandler* handler )
{
	this->ErrorHandler = &IErrorHandler::HandleError;
	this->errorsHandler = handler;
}

