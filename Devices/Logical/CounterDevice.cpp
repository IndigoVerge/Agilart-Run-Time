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

#include "CounterDevice.h"

#define CounterDeviceName "Counter"
#define CounterDeviceType "LogicalDevice"
#define CounterDeviceVersion "0.0.1"
#define CounterDeviceDescr "Counters are used to index, increment or decrement values. The widget increments or decrements the accumulated value at each false to true transition and retains the accumulated value when the instruction goes false. The value is reset by the Reset pin. If the accumulation value is over the maximum range then the overflow will be signaled."
#define CounterDeviceGroup "Logical"
#define CounterDeviceInputConnPoint "Clock"
#define CounterDeviceResetInputConnPoint "Reset"
#define CounterDeviceInputConnPointDescr "Value Input"
#define CounterDeviceOutputConnPoint "Output"
#define CounterDeviceOverflowOutputConnPoint "Overflow"
#define CounterDeviceOutputConnPointDescr "Counter Switch Output"

#define CounterFromParam "From Value"
#define CounterToParam "To Value"
#define CounterStepParam "Step"

CounterDevice::CounterDevice() : BaseDevice()
{
	this->connectionPoints = new list<ConnectionPoint*> ();
	this->pinouts = new std::list<Pinout*>();

	// inputs
	this->inConnPoint = new InConnectionPoint(1, CounterDeviceInputConnPoint, CounterDeviceInputConnPointDescr, BOOL, new bool(false));
	this->inConnPoint->RegisterValueChangedHandler( this );

	this->resetInConnPoint = new InConnectionPoint(2, CounterDeviceResetInputConnPoint, CounterDeviceResetInputConnPoint, BOOL, new bool(false));
	this->resetInConnPoint->RegisterValueChangedHandler( this );

	// outputs
	this->outConnPoint = new OutConnectionPoint(3, CounterDeviceOutputConnPoint, CounterDeviceOutputConnPointDescr, INT, new int(0));
	this->outConnPoint->RegisterRefreshHandler( this );

	this->overflowOutConnPoint = new OutConnectionPoint(4, CounterDeviceOverflowOutputConnPoint, CounterDeviceOverflowOutputConnPoint, BOOL, new bool(false));
	this->overflowOutConnPoint->RegisterRefreshHandler( this );

	this->connectionPoints->push_back(this->inConnPoint);
	this->connectionPoints->push_back(this->resetInConnPoint);
	this->connectionPoints->push_back(this->outConnPoint);
	this->connectionPoints->push_back(this->overflowOutConnPoint);

	DeviceParameter* fromParameter = new DeviceParameter(CounterFromParam, INT, new int(0), "Start counting from this value.");
	DeviceParameter* toParameter = new DeviceParameter(CounterToParam, INT, new int(10), "Raise the Overflow flag when the counter reaches this value.");
	DeviceParameter* stepParameter = new DeviceParameter(CounterStepParam, INT, new int(1), "Incremental step - a positive or negative number which would be added to the current counter value each time the Clock input changes its value.");

	this->AddCustomProperty( fromParameter );
	this->AddCustomProperty( toParameter );
	this->AddCustomProperty( stepParameter );

	this->SetName( CounterDeviceName );

	this->currentInputValue = false;
	this->fromValue = -1;
	this->toValue = -1;
	this->step = 0;
}

CounterDevice::~CounterDevice() {
	// implemented in the BaseDevice
}

const char* CounterDevice::Type() {
	return CounterDeviceType;
}

const char* CounterDevice::Version() {
	return CounterDeviceVersion;
}

const char* CounterDevice::Description() {
	return CounterDeviceDescr;
}

const char* CounterDevice::GroupName() {
	return CounterDeviceGroup;
}

BaseDevice* CounterDevice::CreateInstance() {
	BaseDevice* device = new CounterDevice();
	return device;
}

void CounterDevice::CalculateOutputs()
{
	bool inValue = *(bool*)this->inConnPoint->Value();
	bool resetValue = *(bool*)this->resetInConnPoint->Value();

	if( resetValue == true )
	{
		this->currentValue = this->fromValue;
		this->outConnPoint->SetValue( new int(this->fromValue) );
		this->overflowOutConnPoint->SetValue( new bool(false) );

		return;
	}

	if( currentInputValue == false && inValue == true )
	{
		this->currentValue += this->step;
	}

	if(this->currentValue > this->toValue)
	{
		this->currentValue = this->fromValue;
		this->overflowOutConnPoint->SetValue( new bool(true) );
	}

	this->outConnPoint->SetValue( new int(this->currentValue) );

	currentInputValue = inValue;
}

DeviceParameter CounterDevice::GetParameter( string parameterName )
{
	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, INT, new int( this->currentValue ));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Counter Device",
					parameterName.c_str());

	return DeviceParameter();
}

void CounterDevice::SetParameter( string parameterName, void* value )
{

	if( CounterFromParam == parameterName )
	{
		this->fromValue = Convert::ToInt( value, STRING );
	}
	else if( CounterToParam == parameterName )
	{
		this->toValue = Convert::ToInt( value, STRING );
	}
	else if( CounterStepParam == parameterName )
	{
		this->step = Convert::ToInt( value, STRING );
	}
	else
	{
		EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Counter Device",
				parameterName.c_str());
	}

	if( this->toValue == -1 || this->fromValue == -1 || this->step == 0 )
		return;

	if( this->toValue < this->fromValue )
	{
		if( this->step > 0 )
			this->step *= (-1);
	}
	else
	{
		if( this->step < 0 )
			this->step *= (-1);
	}

	this->currentValue = this->fromValue;
}
