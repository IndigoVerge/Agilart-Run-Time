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
#include "ButtonDevice.h"

#define BUTTON_DEVNAME "Switch"
#define BUTTON_DEVTYPE "GPIOSwitch"
#define BUTTON_VERSION "0.0.1"
#define BUTTON_DESCR "A Switch Widget"
#define BUTTON_GROUP "Common"

ButtonDevice::ButtonDevice() :
	BaseDevice()
{
	this->connectionPoints = new list<ConnectionPoint*>();
	this->pinouts = new list<Pinout*>();

	this->inConnPoint = new InConnectionPoint( 1, "Switch Input",
			"Switch Input", "int", new int(0) );

	this->actuatorInConnPoint = new InConnectionPoint( 2, "Switch Actuator Input",
			"Switch Actuator Input", "int", new int(0) );

	this->inConnPoint->RegisterValueChangedHandler( this );
	this->connectionPoints->push_back( this->inConnPoint );

	this->actuatorInConnPoint->RegisterValueChangedHandler( this );
	this->connectionPoints->push_back( this->actuatorInConnPoint );

	this->outConnPoint = new OutConnectionPoint( 3, "Switch Output",
			"Switch Output", "int", new int(0) );
	this->outConnPoint->RegisterRefreshHandler( this );
	this->connectionPoints->push_back( this->outConnPoint );

	this->currentStatus = 0;
	this->SetName( BUTTON_DEVNAME );
}

ButtonDevice::~ButtonDevice()
{
	// implemented in the BaseDevice
}

void ButtonDevice::CalculateOutputs()
{
	EventLogger::Instance()->WriteVerbose(
			"Switch Device(%d) - LogicalInValueChanged", this->Id() );

	int inValue = *(int*)this->inConnPoint->Value();
	int actuatorValue = *(int*)this->actuatorInConnPoint->Value();
	this->currentStatus = inValue && actuatorValue;

	void* newOutValue = new int( this->currentStatus );
	this->outConnPoint->SetValue( newOutValue );
}

const char* ButtonDevice::Type()
{
	return BUTTON_DEVTYPE;
}

const char* ButtonDevice::Version()
{
	return BUTTON_VERSION;
}

const char* ButtonDevice::Description()
{
	return BUTTON_DESCR;
}

const char* ButtonDevice::GroupName()
{
	return BUTTON_GROUP;
}

BaseDevice* ButtonDevice::CreateInstance()
{
	BaseDevice* device = new ButtonDevice();
	return device;
}

DeviceParameter ButtonDevice::GetParameter( string parameterName )
{
	if( this->currentStatus != 0 && this->currentStatus != 1 )
		return DeviceParameter();

	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, INT, new int( this->currentStatus ));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the GPI Device",
					parameterName.c_str());
	return DeviceParameter();
}

void PinoutEventTriggered( void* sender, std::list<EventParameter> parameters )
{
	if( sender == NULL )
		return;

	BaseDevice* device = (BaseDevice*)sender;
	if( device != NULL )
		device->EventTriggered( parameters );
}
