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

#ifndef DEVICEPARAMETER_H_
#define DEVICEPARAMETER_H_

#include <iostream>

#include "Enums.h"
#include "Convert.h"

using namespace std;

class DeviceParameter
{
private:
	std::string name;
	std::string type;
	std::string stringValue;
	string descr;

	void* value;
	AgilartDataTypes dataType;
	bool isInitialized;

	void Init()
	{
		this->isInitialized = true;
		string* stringPtr = new string( this->stringValue );
		this->dataType = Convert::ToType( this->type );

		switch( this->dataType )
		{
			case BOOL:
				this->value = new bool(Convert::ToBool((void*)stringPtr, STRING));
				break;
			case INT:
				this->value = new int(Convert::ToInt((void*)stringPtr, STRING));
				break;
			case FLOAT:
				this->value = new float(Convert::ToFloat((void*)stringPtr, STRING));
				break;
			case STRING:
				this->value = stringPtr;
				break;
			case DOUBLE:
				this->value = new double(Convert::ToDouble((void*)stringPtr, STRING));
				break;
			case DATETIME:
				break;
			default:
				;
		}
	}

public:
	DeviceParameter()
	{
		this->isInitialized = false;
		this->descr = "Not Available";
	}

	DeviceParameter( std::string name, AgilartDataTypes type, void* value, string description = "Not Available" )
	{
		this->name = name;
		this->dataType = type;
		this->value = value;
		this->stringValue = Convert::ToString( value, type );
		this->isInitialized = true;

		this->descr = description;
	}

	DeviceParameter( std::string name, std::string type, std::string stringValue, string description = "Not Available" )
	{
		this->name = name;
		this->type = type;
		this->stringValue = stringValue;
		this->descr = description;

		this->Init();
	}

	DeviceParameter(const DeviceParameter& param)
	{
		this->isInitialized = param.isInitialized;

		if( param.isInitialized == true )
		{
			this->name = param.name;
			this->type = param.type;
			this->stringValue = param.stringValue;
			this->descr = param.descr;

			this->Init();
		}
	}

	~DeviceParameter()
	{

	}

	bool IsInitialized()
	{
		return this->isInitialized;
	}

	std::string Name()
	{
		return this->name;
	}

	std::string Type()
	{
		return this->type;
	}

	std::string StringValue()
	{
		return this->stringValue;
	}

	void* Value()
	{
		return this->value;
	}

	string Description()
	{
		return this->descr;
	}

	AgilartDataTypes AgilartType()
	{
		return this->dataType;
	}

	std::string ToJson()
	{
		std::string jsonDescr = "{\"" + JsonParameters::DevicePropertyKey + "\":\"" + this->Name() +
				"\",\"" + JsonParameters::DevicePropertyValue + "\":\"" + this->StringValue() +
				"\",\"" + JsonParameters::DevicePropertyType + "\":\"" + this->Type() +
				"\",\"" + JsonParameters::DevicePropertyDescription + "\":\"" + this->Description() + "\"}";

		return jsonDescr;
	}
};

#endif /* DEVICEPARAMETER_H_ */
