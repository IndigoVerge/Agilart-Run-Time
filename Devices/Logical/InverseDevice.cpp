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

#include "InverseDevice.h"

#define DeviceName "Not"
#define DeviceType "LogicalDevice"
#define DeviceVersion "0.0.1"
#define DeviceDescr "Logical Not Widget"
#define DeviceGroup "Logical"
#define DeviceInputConnPoint "Input"
#define DeviceInputConnPointDescr "Inverse Switch Input"
#define DeviceOutputConnPoint "Output"
#define DeviceOutputConnPointDescr "Inverse Switch Output"

#define IntType "int"

InverseDevice::InverseDevice() : BaseDevice()
{
	this->connectionPoints = new list<ConnectionPoint*> ();
	this->pinouts = new std::list<Pinout*>();

	// inputs
	this->inConnPoint = new InConnectionPoint(1, DeviceInputConnPoint, DeviceInputConnPointDescr, BOOL, new bool(false));
	this->inConnPoint->RegisterValueChangedHandler( this );

	// outputs
	this->outConnPoint = new OutConnectionPoint(3, DeviceOutputConnPoint, DeviceOutputConnPointDescr, BOOL, new bool(true));
	this->outConnPoint->RegisterRefreshHandler( this );

	this->connectionPoints->push_back(this->inConnPoint);
	this->connectionPoints->push_back(this->outConnPoint);

	this->SetName( DeviceName );
	this->currentValue = false;
}

InverseDevice::~InverseDevice() {
	// implemented in the BaseDevice
}

const char* InverseDevice::Type() {
	return DeviceType;
}

const char* InverseDevice::Version() {
	return DeviceVersion;
}

const char* InverseDevice::Description() {
	return DeviceDescr;
}

const char* InverseDevice::GroupName() {
	return DeviceGroup;
}

BaseDevice* InverseDevice::CreateInstance() {
	BaseDevice* device = new InverseDevice();
	return device;
}

void InverseDevice::CalculateOutputs()
{
	bool inValue = *(bool*) this->inConnPoint->Value();
	this->currentValue = !(inValue);

	int val = this->currentValue == true ? 1 : 0;

	EventLogger::Instance()->WriteVerbose( "Set Value in %d - %d", this->Id(), val );
	this->outConnPoint->SetValue( new bool(this->currentValue) );
}

DeviceParameter InverseDevice::GetParameter( string parameterName )
{
	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, BOOL, new bool( this->currentValue ));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the NOT Device",
					parameterName.c_str());
	return DeviceParameter();
}
