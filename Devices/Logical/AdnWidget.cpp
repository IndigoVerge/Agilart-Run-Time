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

#include "AdnWidget.h"

AndWidget::AndWidget() : BaseDevice()
{
	this->connectionPoints = new list<ConnectionPoint*> ();

	// inputs
	this->inVal1ConnPoint = new InConnectionPoint(1, ANDDeviceInputVal1ConnPoint, ANDDeviceInputVal1ConnPointDescr, BOOL, new bool(false));
	this->inVal1ConnPoint->RegisterValueChangedHandler( this );

	this->inVal2ConnPoint = new InConnectionPoint(2, ANDDeviceInputVal2ConnPoint, ANDDeviceInputVal2ConnPointDescr, BOOL, new bool(false));
	this->inVal2ConnPoint->RegisterValueChangedHandler( this );

	// outputs
	this->outConnPoint = new OutConnectionPoint(3, ANDDeviceOutputConnPoint, ANDDeviceOutputConnPointDescr, BOOL, new bool(false));
	this->outConnPoint->RegisterRefreshHandler( this );

	this->connectionPoints->push_back(this->inVal1ConnPoint);
	this->connectionPoints->push_back(this->inVal2ConnPoint);
	this->connectionPoints->push_back(this->outConnPoint);

	this->SetName( ANDDeviceName );
	this->currentValue = false;
}

AndWidget::~AndWidget() {
	// implemented in the BaseDevice
}

const char* AndWidget::Type() {
	return ANDDeviceType;
}

const char* AndWidget::Version() {
	return ANDDeviceVersion;
}

const char* AndWidget::Description() {
	return ANDDeviceDescr;
}

const char* AndWidget::GroupName() {
	return ANDDeviceGroup;
}

BaseDevice* AndWidget::CreateInstance() {
	BaseDevice* device = new AndWidget();
	return device;
}

void AndWidget::CalculateOutputs()
{
	bool inValue1 = *(bool*) this->inVal1ConnPoint->Value();
	bool inValue2 = *(bool*) this->inVal2ConnPoint->Value();
	this->currentValue = inValue1 && inValue2;

	this->outConnPoint->SetValue( new bool(this->currentValue) );
}

DeviceParameter AndWidget::GetParameter( string parameterName )
{
	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, BOOL, new bool( this->currentValue ));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the AND Device",
					parameterName.c_str());
	return DeviceParameter();
}
