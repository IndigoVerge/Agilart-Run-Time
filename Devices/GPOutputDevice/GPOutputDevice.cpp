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

#include "GPOutputDevice.h"
#include "EventParameter.h"
#include "Logging/EventLogger.h"

#define GPIO_DEVNAME "Digital Output"
#define GPIO_DEVTYPE "GPODevice"
#define GPIO_VERSION "0.0.1"
#define GPIO_DESCR "General Purpose Output Widget with a physical pin to which it connects. Works best when it is used together with the GPIO LED Driver or other drivers which control a single output value."
#define GPIO_GROUP "Hardware"

using namespace std;

GPOutputDevice::GPOutputDevice() :
	BaseDevice()
{
	this->isOn = 0;
	this->connectionPoints = new list<ConnectionPoint*>();
	this->pinouts = new std::list<Pinout*>();

	this->inputConnPoint = new InConnectionPoint( 1, "Input", "Input Value", INT, new int(0) );
	this->inputConnPoint->RegisterValueChangedHandler( this );

	this->connectionPoints->push_back( this->inputConnPoint );

	// GPIO Pinout Definitions:
	list<CommandParameter*>* cmdInitParams = new list<CommandParameter*>();
	list<CommandParameter*>* cmdSetValueParams = new list<CommandParameter*>();

	cmdInitParams->push_back( new CommandParameter( PORTNUMBER_KEY, NULL, "string" ) );
	cmdSetValueParams->push_back( new CommandParameter( VALUE_KEY, NULL, "int" ) );

	Command cmdInit( INIT_CMD, cmdInitParams );
	Command cmdSetValue( SETVAL_CMD, cmdSetValueParams );
	Command cmdStop( STOP_CMD );

	list<Command>* pinoutSupportedCommands = new list<Command>();
	pinoutSupportedCommands->push_back( cmdInit );
	pinoutSupportedCommands->push_back( cmdSetValue );
	pinoutSupportedCommands->push_back( cmdStop );

	list<Event>* pinoutSupportedEvents = new list<Event>();

	Pinout* pinout = new Pinout( 0, "GPIO Pinout", "Button Device Pinout",
			pinoutSupportedCommands, pinoutSupportedEvents );

	pinout->PinoutEventTriggered = &PinoutEventTriggered;

	this->pinouts->push_back( pinout );
	this->SetName( GPIO_DEVNAME );
}

GPOutputDevice::~GPOutputDevice()
{
	// implemented in the BaseDevice
}

void GPOutputDevice::EventTriggered( list<EventParameter> parameters )
{
	int input = *(int*)this->inputConnPoint->Value();
	this->isOn = input;

	void* newOutValue = new int( input );

	list<CommandParameter*>* params = new list<CommandParameter*>();
	CommandParameter* cp = new CommandParameter( VALUE_KEY, newOutValue, "int" );
	params->push_back( cp );

	Command c( SETVAL_CMD, params );
	this->ExecuteCommand( c );
}

void GPOutputDevice::WidgetInitialized()
{
	BaseDevice::WidgetInitialized();
}

void GPOutputDevice::ReleaseResources()
{
	Command c( STOP_CMD );
	this->ExecuteCommand( c );
}

void GPOutputDevice::CalculateOutputs()
{
	if( this->IsInitialized() == false )
	{
		Command c( INIT_CMD );
		this->ExecuteCommand( c );
	}

	int input = *(int*)this->inputConnPoint->Value();
	this->isOn = input;

	void* newOutValue = new int( input );

	list<CommandParameter*>* params = new list<CommandParameter*> ();
	CommandParameter* cp = new CommandParameter( VALUE_KEY, newOutValue, "int" );
	params->push_back( cp );

	// Send a command to the ( pinout -> MappingManager -> device driver ) to do something - change its state.
	Command c( SETVAL_CMD, params );
	this->ExecuteCommand( c );
}

void GPOutputDevice::ExecuteCommand( Command command )
{
	string logMessage = "GPIO Device - Executing Command "
			+ (string) command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	list<Pinout*>::iterator i;

	for ( i = this->pinouts->begin(); i != this->pinouts->end(); i++ )
	{
		( *i )->ExecuteCommand( command );
	}
}

void GPOutputDevice::SetPortNumber( int value )
{
	this->portNumber = value;
}

const char* GPOutputDevice::Type()
{
	return GPIO_DEVTYPE;
}

const char* GPOutputDevice::Version()
{
	return GPIO_VERSION;
}

const char* GPOutputDevice::Description()
{
	return GPIO_DESCR;
}

const char* GPOutputDevice::GroupName()
{
	return GPIO_GROUP;
}

BaseDevice* GPOutputDevice::CreateInstance()
{
	BaseDevice* device = new GPOutputDevice();
	return device;
}

DeviceParameter GPOutputDevice::GetParameter( string parameterName )
{
	if( this->isOn != 0 && this->isOn != 1 )
		return DeviceParameter();

	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, INT, new int( this->isOn ));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Led Device",
					parameterName.c_str());
	return DeviceParameter();
}

void GPOutputDevice::SetParameter( string parameterName, void* value )
{
	if( this->IsInitialized() == false )
		return;

	list<CommandParameter*>* cmdParams = new list<CommandParameter*>();

	cmdParams->push_back( new CommandParameter( SET_PARAM_NAME, new string(parameterName), STRING_TYPE ) );
	cmdParams->push_back( new CommandParameter( SET_PARAM_VAL, value, STRING_TYPE ) );

	Command cmd( SET_PARAM_CMD, cmdParams );
	this->ExecuteCommand( cmd );
}
