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

#ifndef CONVERT_H_
#define CONVERT_H_

#include "Enums.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Convert {
public:
	static bool ToBool( void * value, AgilartDataTypes );
	static int ToInt( void * value, AgilartDataTypes );
	static string ToString( void * value, AgilartDataTypes );
	static float ToFloat( void * value, AgilartDataTypes );
	static double ToDouble(  void * value, AgilartDataTypes );
	// static ctime ToDateTime(  void * value, AgilartDataTypes );

	static bool SupportsConversion( AgilartDataTypes v1, AgilartDataTypes v2 );
	static bool AreEqual( void* value1, void* value2, AgilartDataTypes type );
	static void* ToTypeInstance( void* value, AgilartDataTypes fromType, AgilartDataTypes toType );

	static AgilartDataTypes GetBaseType( AgilartDataTypes type1, AgilartDataTypes type2 );

	//toDo: static get type with higher precedence

	static AgilartDataTypes ToType( string );
};

#endif /* CONVERT_H_ */
