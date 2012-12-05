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

#ifndef EVENTPARAMETER_H_
#define EVENTPARAMETER_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include "JsonParameters.h"
using namespace std;

struct EventParameter
{
	public:

		EventParameter()
		{
		}

		EventParameter( const EventParameter& param )
		{
			this->Key = param.Key;
			this->Type = param.Type;
			this->Value = param.Value;
		}

		EventParameter( const char* key, int value, const char* type )
		{
			this->Key = key;
			this->Value = value;
			this->Type = type;
		}

		~EventParameter()
		{
		}

		const char* Key;
		int Value;
		const char* Type;

		EventParameter Clone()
		{
			EventParameter clone( this->Key, this->Value, this->Type );
			return clone;
		}

		string ToJson()
		{
			string value = "";
			char buf[256];
			sprintf( buf, "%d", this->Value );
			value = buf;

			string jsonDescr = "{\"" + JsonParameters::Key
					+ "\": \"" + this->Key + "\", \""
					+ JsonParameters::DataType + "\": \"" + this->Type + "\", \""
					+ JsonParameters::Value + "\": \"" + value + "\"}";

			return jsonDescr;
		}
};

#endif /* EVENTPARAMETER_H_ */
