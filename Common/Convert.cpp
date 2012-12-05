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

#include "Convert.h"

using namespace std;

bool Convert::ToBool( void * value, AgilartDataTypes type )
{
	if( value == NULL || Convert::SupportsConversion( type, BOOL ) == false )
		return false;

	bool resultValue = false;
	string stringValue;
	int intvalue;

	switch( type )
	{
		case BOOL:
			resultValue = *(bool*)value;
			break;

		case STRING:
			stringValue = *(string*)value;

			if( stringValue == "true" || stringValue == "True" )
				resultValue = true;
			break;

		case INT:
			intvalue = *(int*)value;

			if( intvalue == 1 )
				resultValue = true;
			break;

		default:
			;
	}

	return resultValue;
}

int Convert::ToInt( void * value, AgilartDataTypes type )
{
	if( value == NULL || Convert::SupportsConversion( type, INT ) == false )
		return 0;

	int resultValue = 0;
	string stringValue;
	float floatValue;
	bool boolValue;

	switch( type )
	{
		case BOOL:
			boolValue = *(bool*)value;
			resultValue = boolValue == true ? 1 : 0;
			break;

		case STRING:
			stringValue = *(string*)value;
			resultValue = atoi( stringValue.c_str() );
			break;

		case INT:
			resultValue = *(int*)value;
			break;

		case FLOAT:
			floatValue = *(float*)value;
			resultValue = (int)floatValue;
			break;

		default:
			;
	}

	return resultValue;
}

string Convert::ToString( void * value, AgilartDataTypes type )
{
	if( value == NULL || Convert::SupportsConversion( type, STRING ) == false)
		return "";

	string resultValue = "";
	int intValue;
	float floatValue;
	bool boolValue;
	char buffer[32];

	switch( type )
	{
		case BOOL:
			boolValue = *(bool*)value;
			resultValue = boolValue == true ? "True" : "False";
			break;

		case STRING:
			resultValue = *(string*)value;
			break;

		case INT:
			intValue = *(int*)value;
			sprintf( buffer, "%d", intValue );
			resultValue = (string)buffer;
			break;

		case FLOAT:
			floatValue = *(float*)value;
			sprintf( buffer, "%.2f", floatValue );
			resultValue = (string)buffer;
			break;

		default:
			;
	}

	return resultValue;
}

float Convert::ToFloat( void * value, AgilartDataTypes type )
{
	if( value == NULL || Convert::SupportsConversion( type, FLOAT ) == false)
		return 0;

	float resultValue = 0.0f;
	string stringValue;
	int intValue;
	bool boolValue;

	switch( type )
	{
		case BOOL:
			boolValue = *(bool*)value;
			resultValue = boolValue == true ? 1.0f : 0.0f;
			break;

		case STRING:
			stringValue = *(string*)value;
			resultValue = atof( stringValue.c_str() );
			break;

		case INT:
			intValue = *(int*)value;
			resultValue = (float)intValue;
			break;

		case FLOAT:
			resultValue = *(float*)value;
			break;

		default:
			;
	}

	return resultValue;
}

double Convert::ToDouble(  void * value, AgilartDataTypes type )
{
	if( value == NULL || Convert::SupportsConversion( type, DOUBLE ) == false)
		return 0;

	double resultValue = 0.0f;
	string stringValue;
	int intValue;
	float floatValue;
	bool boolValue;

	switch( type )
	{
		case BOOL:
			boolValue = *(bool*)value;
			resultValue = boolValue == true ? 1.0f : 0.0f;
			break;

		case STRING:
			stringValue = *(string*)value;
			resultValue = (double)(atof( stringValue.c_str() ));
			break;

		case INT:
			intValue = *(int*)value;
			resultValue = (double)intValue;
			break;

		case FLOAT:
			floatValue = *(float*)value;
			resultValue = (double)floatValue;
			break;

		case DOUBLE:
			resultValue = *(double*)value;
			break;

		default:
			;
	}

	return resultValue;
}

AgilartDataTypes Convert::ToType( string type )
{
	AgilartDataTypes resultType = UNDEFINED;

	if( type == "bool" )
	{
		resultType = BOOL;
	}
	else if( type == "string" )
	{
		resultType = STRING;
	}
	else if(type == "float" )
	{
		resultType = FLOAT;
	}
	else if(type == "int")
	{
		resultType = INT;
	}
	else if(type == "double")
	{
		resultType = DOUBLE;
	}
	else if(type == "datetime")
	{
		resultType = DATETIME;
	}
	else if(type == "any")
	{
		resultType = ANY;
	}

	return resultType;
}

bool Convert::SupportsConversion( AgilartDataTypes v1, AgilartDataTypes v2 )
{
	return true;
}

bool Convert::AreEqual( void* value1, void* value2, AgilartDataTypes type )
{
	string value1AsString = Convert::ToString( value1, type );
	string value2AsString = Convert::ToString( value2, type );

	return value1AsString == value2AsString;
}

void* Convert::ToTypeInstance( void* value, AgilartDataTypes fromType, AgilartDataTypes toType )
{
	void* resultValue = NULL;

	if( toType == ANY )
		toType = fromType;

	switch( toType )
	{
		case BOOL:
			resultValue = new bool( Convert::ToBool( value, fromType ) );
			break;

		case STRING:
			resultValue = new string( Convert::ToString( value, fromType ) );
			break;

		case INT:
			resultValue = new int( Convert::ToInt( value, fromType ) );
			break;

		case FLOAT:
			resultValue = new float( Convert::ToFloat( value, fromType ) );
			break;

		case DOUBLE:
			resultValue = new double( Convert::ToDouble( value, fromType ) );
			break;

		default:
			;
	}

	return resultValue;
}

AgilartDataTypes Convert::GetBaseType( AgilartDataTypes type1, AgilartDataTypes type2 )
{
	if( Convert::SupportsConversion(type1, type2) == false )
		return UNDEFINED;

	AgilartDataTypes base = UNDEFINED;

	if( type1 == type2 ) {
		base = type1;
	} else if( ( type1 == INT && type2 == FLOAT ) || ( type2 == INT && type1 == FLOAT ) ) {
		base = FLOAT;
	} else if( ( type2 == DOUBLE && type1 == FLOAT ) || ( type2 == DOUBLE && type1 == FLOAT ) ) {
		base = FLOAT;
	}

	return base;
}
