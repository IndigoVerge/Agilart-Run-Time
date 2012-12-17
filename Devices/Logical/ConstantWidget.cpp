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

#include "ConstantWidget.h"

#define DEV_NAME "Constant Value"
#define DEV_TYPE "LogicalDevice"
#define DEV_VERSION "0.0.1"
#define DEV_DESCR "A Constant Value Widget"
#define DEV_GROUP "Common"

#define VALUE_PARAM "Value"
#define VALUE_TYPE_PARAM "Data Type"

ConstantDevice::ConstantDevice() :
	BaseDevice()
{
	this->connectionPoints = new list<ConnectionPoint*>();
	this->pinouts = new list<Pinout*>();

	this->outConnPoint = new OutConnectionPoint( 1, "Output",
			"Constant Output", "int", new int(-1) );
	this->outConnPoint->RegisterRefreshHandler( this );
	this->connectionPoints->push_back( this->outConnPoint );

	DeviceParameter* valueParam = new DeviceParameter( VALUE_PARAM, STRING, new string(""), "Enter the value of the Constant Widget. It will remain the same during the program execution process." );
	DeviceParameter* valueTypeParam = new DeviceParameter( VALUE_TYPE_PARAM, STRING, new string(""), "Enter value type. For now choose from: int, double, bool, string, datetime, any" );

	this->AddCustomProperty( valueParam );
	this->AddCustomProperty( valueTypeParam );

	this->SetName( DEV_NAME );
	this->type = UNDEFINED;
	this->stringValue = NULL;
}

ConstantDevice::~ConstantDevice()
{
	// implemented in the BaseDevice
}
void ConstantDevice::CalculateOutputs()
{
	this->outConnPoint->SetValue( this->value );
}

void ConstantDevice::WidgetInitialized()
{
	BaseDevice::WidgetInitialized();
}

void ConstantDevice::UpdateOutconnectionsValues( void* newOutValue )
{
	this->outConnPoint->SetValue( newOutValue );
}

const char* ConstantDevice::Type()
{
	return DEV_TYPE;
}

const char* ConstantDevice::Version()
{
	return DEV_VERSION;
}

const char* ConstantDevice::Description()
{
	return DEV_DESCR;
}

const char* ConstantDevice::GroupName()
{
	return DEV_GROUP;
}

BaseDevice* ConstantDevice::CreateInstance()
{
	BaseDevice* device = new ConstantDevice();
	return device;
}

DeviceParameter ConstantDevice::GetParameter( string parameterName )
{
	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, this->type, this->value );
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Constant Device",
					parameterName.c_str());
	return DeviceParameter();
}

void ConstantDevice::SetParameter( string parameterName, void* value )
{
	if( VALUE_PARAM == parameterName )
	{
		this->stringValue = value;
	}
	else if( VALUE_TYPE_PARAM == parameterName )
	{
		string stringType = *(string*)value;
		this->type = Convert::ToType( stringType );
	}
	else
	{
		EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Constant Widget",
				parameterName.c_str());
	}

	if( this->stringValue != NULL && this->type != UNDEFINED )
	{
		this->value = Convert::ToTypeInstance( this->stringValue, STRING, this->type );

		this->outConnPoint->SetDataType( this->type );
	}
}
