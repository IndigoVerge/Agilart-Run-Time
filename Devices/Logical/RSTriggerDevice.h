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

#ifndef RSTRIGGER_H_
#define RSTRIGGER_H_

#include <iostream>

#include "UPCBDescriptors.h"
#include "Logging/EventLogger.h"
#include "BaseDevice.h"
#include "OutConnectionPoint.h"
#include "InConnectionPoint.h"

#define DeviceName "R/S Trigger Switch"
#define DeviceType "LogicalDevice"
#define DeviceDescr "S/R Trigger Switch (or a flip-flop) is a logical widget with two input pins (a Set pin and a Reset pin) and one output pin. When the S/R Trigger receives True on its Set pin and the Reset Pin is False, the output would also be assigned a value of True. However when you change the value of the Reset pin to True, the output pin will be turned back to False."
#define DeviceGroup "Common"
#define DeviceVersion "0.0.1"
#define DeviceSetInputConnPoint "Set"
#define DeviceSetInputConnPointDescr "R/S Trigger Set Input"
#define DeviceResetInputConnPoint "Reset"
#define DeviceResetInputConnPointDescr "R/S Trigger Reset Input"
#define DeviceOutputConnPoint "Output"
#define DeviceOutputConnPointDescr "R/S Trigger Output"

#define IntType "int"

class RSTriggerDevice: public BaseDevice
{
	public:
		RSTriggerDevice();
		virtual ~RSTriggerDevice();

		virtual const char* Type();
		virtual const char* Version();
		virtual const char* Description();
		virtual const char* GroupName();

		virtual BaseDevice* CreateInstance();

	protected:
		virtual void CalculateOutputs();

	private:
		InConnectionPoint* setInConnPoint;
		InConnectionPoint* resetInConnPoint;
		OutConnectionPoint* outConnPoint;
};

#endif /* RSTRIGGER_H_ */
