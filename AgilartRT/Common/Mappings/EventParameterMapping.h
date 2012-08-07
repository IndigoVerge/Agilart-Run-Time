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
#ifndef EVENTPARAMETERMAPPING_H_
#define EVENTPARAMETERMAPPING_H_

#include <string>
using namespace std;

class EventParameterMapping
{
	private:
		string source;
		string target;
		void* defaultValue;
		string defaultType;

	public:
		EventParameterMapping( string source, string target )
		{
			this->source = source;
			this->target = target;
			this->defaultValue = NULL;
			this->defaultType = "";
		}

		EventParameterMapping( string source, string target, void* value, string type )
		{
			this->source = source;
			this->target = target;
			this->defaultValue = value;
			this->defaultType = type;
		}

		EventParameterMapping( EventParameterDescriptor* mapping )
		{
			string value;

			DescriptorsUtil::GetStringProperty( mapping->Properties,
											"Source", &this->source );
			DescriptorsUtil::GetStringProperty( mapping->Properties,
											"Target", &this->target );
			DescriptorsUtil::GetStringProperty( mapping->Properties,
											"Type", &this->defaultType );
			DescriptorsUtil::GetStringProperty( mapping->Properties,
											"Value", &value );

			if( this->defaultType == "int" )
			{
				int* intValue = new int( atoi(value.c_str()));
				this->defaultValue = intValue;
			}
			else
			{
				string* stringValue = new string();
				stringValue->assign(value);
				this->defaultValue = stringValue;
			}
		}

		EventParameterMapping( const EventParameterMapping& param )
		{
			this->source = param.source;
			this->target = param.target;

			if(param.defaultValue == NULL)
				return;

			this->defaultType = param.defaultType;

			if( this->defaultType == "int" )
			{
				int* intValue = (int*)param.defaultValue;
				this->defaultValue = new int(*intValue);
			}
			else
			{
				string* stringValue = (string*)param.defaultValue;
				string* defaultString = new string();
				defaultString->assign(*stringValue);
				this->defaultValue = defaultString;
			}
		}

		~EventParameterMapping()
		{
		}


		string Source()
		{
			return source;
		}

		string Target()
		{
			return target;
		}

		void* DefaultValue()
		{
			return defaultValue;
		}

		string DefaultType()
		{
			return defaultType;
		}
};

#endif /* EVENTPARAMETERMAPPING_H_ */
