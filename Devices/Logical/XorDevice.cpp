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

#include "XorDevice.h"

XorDevice::XorDevice() : BaseDevice()
{
	this->connectionPoints = new list<ConnectionPoint*> ();

	// inputs
	this->inVal1ConnPoint = new InConnectionPoint(1, XORDeviceInputVal1ConnPoint, XORDeviceInputVal1ConnPointDescr, BOOL, new bool(false));
	this->inVal1ConnPoint->RegisterValueChangedHandler( this );

	this->inVal2ConnPoint = new InConnectionPoint(2, XORDeviceInputVal2ConnPoint, XORDeviceInputVal2ConnPointDescr, BOOL, new bool(false));
	this->inVal2ConnPoint->RegisterValueChangedHandler( this );

	// outputs
	this->outConnPoint = new OutConnectionPoint(3, XORDeviceOutputConnPoint, XORDeviceOutputConnPointDescr, BOOL, new bool(false));
	this->outConnPoint->RegisterRefreshHandler( this );

	this->connectionPoints->push_back(this->inVal1ConnPoint);
	this->connectionPoints->push_back(this->inVal2ConnPoint);
	this->connectionPoints->push_back(this->outConnPoint);

	this->SetName( XORDeviceName );
}

XorDevice::~XorDevice() {
	// implemented in the BaseDevice
}

const char* XorDevice::Type() {
	return XORDeviceType;
}

const char* XorDevice::Version() {
	return XORDeviceVersion;
}

const char* XorDevice::Description() {
	return XORDeviceDescr;
}

const char* XorDevice::GroupName() {
	return XORDeviceGroup;
}

BaseDevice* XorDevice::CreateInstance() {
	BaseDevice* device = new XorDevice();
	return device;
}

void XorDevice::CalculateOutputs()
{
	bool inValue1 = *(bool*) this->inVal1ConnPoint->Value();
	bool inValue2 = *(bool*) this->inVal2ConnPoint->Value();
	bool outValue = (inValue1 && !inValue2) || (!inValue1 && inValue2);

	this->outConnPoint->SetValue( new bool(outValue) );
}
