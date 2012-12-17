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

#include "ComparatorWidget.h"

#define DEV_NAME "Comparator"
#define DEV_TYPE "LogicalDevice"
#define DEV_VERSION "0.0.1"
#define DEV_DESCR "A Comparator Widget"
#define DEV_GROUP "Common"

#define OPERATION_PARAM "Operation"

ComparatorDevice::ComparatorDevice() :
	BaseDevice()
{
	this->connectionPoints = new list<ConnectionPoint*>();
	this->pinouts = new list<Pinout*>();

	this->inValue1ConnPoint = new InConnectionPoint( 1, "Value 1", "Value 1", ANY, NULL );
	this->inValue2ConnPoint = new InConnectionPoint( 2, "Value 2", "Value 2", ANY, NULL );

	this->inValue1ConnPoint->RegisterValueChangedHandler( this );
	this->connectionPoints->push_back(this->inValue1ConnPoint);

	this->inValue2ConnPoint->RegisterValueChangedHandler( this );
	this->connectionPoints->push_back(this->inValue2ConnPoint);

	this->outConnPoint = new OutConnectionPoint( 3, "Output", "Comparator Output", BOOL, new bool(false) );
	this->outConnPoint->RegisterRefreshHandler( this );
	this->connectionPoints->push_back( this->outConnPoint );

	DeviceParameter* operationParam = new DeviceParameter( OPERATION_PARAM, STRING, new string(""), "Enter the target operation. For now choose from: <, >, =, <= or >=" );
	this->AddCustomProperty( operationParam );

	this->SetName( DEV_NAME );
}

ComparatorDevice::~ComparatorDevice()
{
	// implemented in the BaseDevice
}

void ComparatorDevice::CalculateOutputs()
{
	this->currentStatus = this->GetComparisonResult();

	void* newOutValue = new bool( this->currentStatus);
	this->outConnPoint->SetValue( newOutValue );
}

const char* ComparatorDevice::Type()
{
	return DEV_TYPE;
}

const char* ComparatorDevice::Version()
{
	return DEV_VERSION;
}

const char* ComparatorDevice::Description()
{
	return DEV_DESCR;
}

const char* ComparatorDevice::GroupName()
{
	return DEV_GROUP;
}

BaseDevice* ComparatorDevice::CreateInstance()
{
	BaseDevice* device = new ComparatorDevice();
	return device;
}

DeviceParameter ComparatorDevice::GetParameter( string parameterName )
{
	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, BOOL, new bool( this->currentStatus ));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the GPI Device",
					parameterName.c_str());
	return DeviceParameter();
}

void ComparatorDevice::SetParameter( string parameterName, void* value )
{
	if( OPERATION_PARAM == parameterName )
	{
		string operationValue = *(string*)value;
		this->operation = this->GetOperation(operationValue);
	}
	else
	{
		EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Comparator Widget",
				parameterName.c_str());
	}
}

ComparatorOperations ComparatorDevice::GetOperation(string value)
{
	ComparatorOperations result = Undefined;

	if( value == "=" )	{
		result = Equal;
	} else if( value == "<") {
		result = LessThan;
	} else if( value == "<=") {
		result = LessOrEqual;
	} else if( value == ">") {
		result = MoreThan;
	} else if( value == ">=") {
		result = MoreOrEqual;
	}

	return result;
}

bool ComparatorDevice::GetComparisonResult()
{
	bool operationResult = false;
	AgilartDataTypes currentDataType = Convert::GetBaseType( this->inValue1ConnPoint->DataType(), this->inValue2ConnPoint->DataType() );
	int intVal1;
	int intVal2;
	float val1;
	float val2;

	switch( currentDataType )
	{
		case INT:
			intVal1 = Convert::ToInt(this->inValue1ConnPoint->Value(), this->inValue1ConnPoint->DataType());
			intVal2 = Convert::ToInt(this->inValue2ConnPoint->Value(), this->inValue2ConnPoint->DataType());

			switch(this->operation)
			{
				case LessThan:
					operationResult = intVal1 < intVal2 ? true : false;
					break;
				case MoreThan:
					operationResult = intVal1 > intVal2 ? true : false;
					break;
				case LessOrEqual:
					operationResult = intVal1 <= intVal2 ? true : false;
					break;
				case MoreOrEqual:
					operationResult = intVal1 >= intVal2 ? true : false;
					break;
				case Equal:
					operationResult = intVal1 == intVal2 ? true : false;
					break;
				default:
					break;
			}
			break;
		case FLOAT:
			val1 = Convert::ToFloat(this->inValue1ConnPoint->Value(), this->inValue1ConnPoint->DataType());
			val2 = Convert::ToFloat(this->inValue2ConnPoint->Value(), this->inValue2ConnPoint->DataType());

			switch(this->operation)
			{
				case LessThan:
					operationResult = val1 < val2 ? true : false;
					break;
				case MoreThan:
					operationResult = val1 > val2 ? true : false;
					break;
				case LessOrEqual:
					operationResult = val1 <= val2 ? true : false;
					break;
				case MoreOrEqual:
					operationResult = val1 >= val2 ? true : false;
					break;
				case Equal:
					operationResult = val1 == val2 ? true : false;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

	return operationResult;
}


