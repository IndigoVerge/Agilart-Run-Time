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

#ifndef COMMANDPARAMETER_H_
#define COMMANDPARAMETER_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include "JsonParameters.h"
using namespace std;

class CommandParameter {
public:

	CommandParameter() {
	}

	CommandParameter(string key, void* value, string type) {
		this->Key = key;
		this->Value = value;
		this->Type = type;
	}

	CommandParameter(const CommandParameter& param) {
		this->Key = param.Key;
		this->Type = param.Type;

		if (param.Value == NULL) {
			this->Value = NULL;
		} else if (this->Type == "int") {
			int* intValue = (int*) param.Value;
			this->Value = new int(*intValue);
		} else if (this->Type == "string") {
			string* stringValue = (string*) param.Value;
			string* defaultString = new string();
			defaultString->assign(*stringValue);
			this->Value = defaultString;
		}
	}

	~CommandParameter() {
		if (this->Value == NULL) {
			return;
		} else if (this->Type == "int") {
			int* intValue = (int*) this->Value;
			delete intValue;

			this->Value = NULL;
		} else if (this->Type == "string") {
			string* stringValue = (string*) this->Value;
			delete stringValue;

			this->Value = NULL;
		}
	}

	string Key;
	void* Value;
	string Type;

	CommandParameter* Clone()
	{
		void* cloneValue;

		if (this->Value == NULL) {
			cloneValue = NULL;
		} else if (this->Type == "int") {
			int* intValue = (int*) this->Value;
			cloneValue = new int(*intValue);
		} else if (this->Type == "string") {
			string* stringValue = (string*) this->Value;
			string* defaultString = new string();
			defaultString->assign(*stringValue);
			cloneValue = defaultString;
		}

		CommandParameter *clone = new CommandParameter(this->Key, cloneValue,
				this->Type);
		return clone;
	}

	string ToJson()
	{
		string value = "";

		if( this->Value == NULL )
		{
		}
		else if( this->Type == "int" )
		{
			int intValue = *((int*) this->Value);
			char buf[256];
			sprintf( buf, "%d", intValue );
			value = buf;
		}
		else if (this->Type == "string")
		{
			value = *((string*) this->Value);
		}

		string jsonDescr = "{\"" + JsonParameters::Key
				+ "\": \"" + this->Key + "\", \""
				+ JsonParameters::DataType + "\": \"" + this->Type + "\", \""
				+ JsonParameters::Value + "\": \"" + value + "\"}";

		return jsonDescr;
	}
};


#endif /* COMMANDPARAMETER_H_ */
