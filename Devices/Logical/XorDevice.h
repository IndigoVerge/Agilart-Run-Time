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

#ifndef XORDEVICE_H_
#define XORDEVICE_H_

#include "BaseDevice.h"

#define XORDeviceName "XOR"
#define XORDeviceType "LogicalDevice"
#define XORDeviceVersion "0.0.1"
#define XORDeviceDescr "The XOR gate is a digital logic gate that implements an exclusive or; that is, a true output (1) results if one, and only one, of the inputs to the gate is true (1)."
#define XORDeviceGroup "Logical"
#define XORDeviceInputVal1ConnPoint "Value 1"
#define XORDeviceInputVal1ConnPointDescr "First Operand"
#define XORDeviceInputVal2ConnPoint "Value 2"
#define XORDeviceInputVal2ConnPointDescr "Second Operand"
#define XORDeviceOutputConnPoint "Output"
#define XORDeviceOutputConnPointDescr "Output Value"

class XorDevice : public BaseDevice
{
	public:
		XorDevice();
		virtual ~XorDevice();

		virtual const char* Type();
		virtual const char* Version();
		virtual const char* Description();
		virtual const char* GroupName();

		virtual BaseDevice* CreateInstance();

	protected:
		virtual void CalculateOutputs();

	private:
		InConnectionPoint* inVal1ConnPoint;
		InConnectionPoint* inVal2ConnPoint;
		OutConnectionPoint* outConnPoint;
};
#endif /* XORDEVICE_H_ */
