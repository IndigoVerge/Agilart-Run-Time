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

#include "RSTriggerDevice.h"

RSTriggerDevice::RSTriggerDevice() : BaseDevice()
{
	this->connectionPoints = new list<ConnectionPoint*> ();
	this->pinouts = new std::list<Pinout*>();

	// inputs
	this->setInConnPoint = new InConnectionPoint(1, DeviceSetInputConnPoint, DeviceSetInputConnPointDescr, IntType, new int(0));
	this->setInConnPoint->RegisterValueChangedHandler( this );

	this->resetInConnPoint = new InConnectionPoint(2, DeviceResetInputConnPoint, DeviceResetInputConnPointDescr, IntType, new int(0));
	this->resetInConnPoint->RegisterValueChangedHandler( this );

	// outputs
	this->outConnPoint = new OutConnectionPoint(3, DeviceOutputConnPoint, DeviceOutputConnPointDescr, IntType, new int(0));
	this->outConnPoint->RegisterRefreshHandler( this );

	this->connectionPoints->push_back(this->setInConnPoint);
	this->connectionPoints->push_back(this->resetInConnPoint);
	this->connectionPoints->push_back(this->outConnPoint);

	this->SetName( DeviceName );
}

RSTriggerDevice::~RSTriggerDevice() {
	// TODO Auto-generated destructor stub
}

const char* RSTriggerDevice::Type() {
	return DeviceType;
}

const char* RSTriggerDevice::Version() {
	return DeviceVersion;
}

const char* RSTriggerDevice::Description() {
	return DeviceDescr;
}

const char* RSTriggerDevice::GroupName() {
	return DeviceGroup;
}

BaseDevice* RSTriggerDevice::CreateInstance() {
	BaseDevice* device = new RSTriggerDevice();
	return device;
}

void RSTriggerDevice::CalculateOutputs()
{
	int setInValue = *(int*) this->setInConnPoint->Value();
	int resetInValue = *(int*) this->resetInConnPoint->Value();

	if( setInValue == 0 && resetInValue == 0 )
	{
		// No change.
		return;
	}

	int outValue = !(resetInValue) && setInValue;
	this->outConnPoint->SetValue( new int(outValue) );
}

