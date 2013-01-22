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

#include "TemperatureDevice.h"

#define TMP_CHANGED "Temperature changed"
#define TMP_DEVNAME "Temperature Sensor"
#define TMP_DEVTYPE "TmpDevice"
#define TMP_VERSION "0.0.1"
#define TMP_DESCR "Temperature Sensor Widget"
#define TMP_GROUP "Hardware"
#define TMP_VALUE "Value"

TemperatureDevice::TemperatureDevice() :
	BaseDevice()
{
	this->connectionPoints = new list<ConnectionPoint*> ();
	this->pinouts = new std::list<Pinout*>();

	this->outputConnPoint = new OutConnectionPoint( 1, "Output",
			"Temperature Sensor Device Output", BOOL, new bool(false) );

	this->outputConnPoint->RegisterRefreshHandler( this );
	this->connectionPoints->push_back( this->outputConnPoint );

	// Button Pionout definition:
	Command cmdInit( INIT_CMD );
	Command cmdStop( STOP_CMD );

	list<Command>* pinoutSupportedCommands = new list<Command>();
	pinoutSupportedCommands->push_back( cmdInit );
	pinoutSupportedCommands->push_back( cmdStop );

	list<Event>* pinoutSupportedEvents = new list<Event>();
	list<EventParameter>* eventParams = new list<EventParameter>();
	eventParams->push_back( EventParameter( TMP_VALUE, 0, "float" ) );

	Event eventClick( TMP_CHANGED, eventParams );
	pinoutSupportedEvents->push_back( eventClick );

	this->pinout = new Pinout( 0, "Temperature Sensor Pinout", "Sensor",
			pinoutSupportedCommands, pinoutSupportedEvents );
	pinout->PinoutEventTriggered = &PinoutEventTriggered;

	this->pinouts->push_back( pinout );
	this->currentPinoutStatus = 0;
	this->SetName( TMP_DEVNAME );
}

TemperatureDevice::~TemperatureDevice()
{
	// implemented in the BaseDevice
}

void TemperatureDevice::WidgetInitialized()
{
	BaseDevice::WidgetInitialized();

	Command c( INIT_CMD );
	this->ExecuteCommand( c );
}

void TemperatureDevice::ReleaseResources()
{
	Command c( STOP_CMD );
	this->ExecuteCommand( c );
}

void TemperatureDevice::CalculateOutputs()
{
	void* resultPointer = new float(this->currentPinoutStatus );
	this->outputConnPoint->SetValue( resultPointer );
}

void TemperatureDevice::EventTriggered( list<EventParameter> parameters )
{
	EventLogger::Instance()->WriteVerbose( "Temperature Sensor - %d - EventTriggered", this->Id() );

	list<EventParameter>::iterator pIterator;
	string val = TMP_VALUE;

	for ( pIterator = parameters.begin(); pIterator != parameters.end(); pIterator++ )
	{
		if ( strcmp( ( *pIterator ).Key, val.c_str() ) != 0 )
			continue;
		this->currentPinoutStatus = ( *pIterator ).Value;
		break;
	}

	void* resultPointer = new float(this->currentPinoutStatus );
	this->outputConnPoint->SetValue( resultPointer );
}

void TemperatureDevice::ExecuteCommand( Command command )
{
	string logMessage = "Temperature Sensor - Executing Command "
			+ (string) command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	this->pinout->ExecuteCommand( command );
}

const char* TemperatureDevice::Type()
{
	return TMP_DEVTYPE;
}

const char* TemperatureDevice::Version()
{
	return TMP_VERSION;
}

const char* TemperatureDevice::Description()
{
	return TMP_DESCR;
}

const char* TemperatureDevice::GroupName()
{
	return TMP_GROUP;
}

BaseDevice* TemperatureDevice::CreateInstance()
{
	BaseDevice* device = new TemperatureDevice();
	return device;
}

DeviceParameter TemperatureDevice::GetParameter( string parameterName )
{
	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, FLOAT, new float(currentPinoutStatus));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s is not recognized by the Temperature Device",
					parameterName.c_str());
	return DeviceParameter();
}

