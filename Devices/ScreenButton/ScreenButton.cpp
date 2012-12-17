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
#include "ScreenButton.h"

#define VALUE_KEY "Value"
#define NOTSET -1

#define SCRBTN_DEVNAME "Html Actuator"
#define SCRBTN_DEVTYPE "GPIDevice"
#define SCRBTN_VERSION "0.0.1"
#define SCRBTN_DESCR "Html controlled input widget. You can use HTML javaScript code to alter its value when the user pushes a button on a html page."
#define SCRBTN_GROUP "Other"

ScreenButton::ScreenButton() :
	BaseDevice()
{
	this->currentStatus = 0;
	this->connectionPoints = new list<ConnectionPoint*> ();
	this->pinouts = new std::list<Pinout*>();

	this->outConnPoint = new OutConnectionPoint( 2, "Output",
			"Screen Button Device Output", "int", new int(0) );
	this->outConnPoint->RegisterRefreshHandler( this );
	this->connectionPoints->push_back( this->outConnPoint );

	this->SetName( SCRBTN_DEVNAME );
}

ScreenButton::~ScreenButton()
{
	// implemented in the BaseDevice
}

void ScreenButton::CalculateOutputs()
{
	EventLogger::Instance()->WriteVerbose(
			"ScreenButton Device - %d - LogicalInValueChanged", this->Id() );

	int outValue = this->currentStatus;
	void* resultPointer = new int( outValue );

	this->outConnPoint->SetValue( resultPointer );
}

const char* ScreenButton::Type()
{
	return SCRBTN_DEVTYPE;
}

const char* ScreenButton::Version()
{
	return SCRBTN_VERSION;
}

const char* ScreenButton::Description()
{
	return SCRBTN_DESCR;
}

const char* ScreenButton::GroupName()
{
	return SCRBTN_GROUP;
}

BaseDevice* ScreenButton::CreateInstance()
{
	BaseDevice* device = new ScreenButton();
	return device;
}

DeviceParameter ScreenButton::GetParameter( string parameterName )
{
	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, INT, new int( this->currentStatus ));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Screen Button Device",
					parameterName.c_str());
	return DeviceParameter();
}

void ScreenButton::SetParameter( string parameterName, void* value )
{
	if( VALUE_KEY == parameterName )
	{
		this->currentStatus = atoi( ( *(string*)value ).c_str() );
		this->CalculateOutputs();
	}
	else
	{
		EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Screen Button Device",
				parameterName.c_str());
	}
}
