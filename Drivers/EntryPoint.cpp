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
/*
 * EntryPoint.cpp
 *
 *  Created on: Jul 28, 2011
 *      Author: developer
 */

#include <list>
#include "BaseDriver.h"
#include "Logging/EventLogger.h"

#include "GPIOKeyDriver/GPIOKeyDriver.h"
#include "SysfsGenericGPIODriver/SysfsGenericGPIODriver.h"
#include "GPIOLedDriver/GPIOLedDriver.h"
#include "ADCDriver/ADCDriver.h"
#include "UltraHeatDriver/UltraHeatDriver.h"
#include "RaspiGPIO/RaspiGPInputDriver.h"
#include "RaspiGPIO/RaspiGPOutputDriver.h"
#include "SysFSInputDriver/SysfsInputDriver.h"
#include "SysFSOutputDriver/SysfsOutputDriver.h"
//#include "ZWaveSwitchDriver/ZWaveSwitchDriver.h"
#include "FeedSocketDriver/FeedSocketDriver.h"

using namespace std;

extern "C" list<BaseDriver*> CreateDrivers()
{
	EventLogger::Instance()->WriteDebug( "Executing CreateDrivers!" );

	list<BaseDriver*> drivers;

	GPIOKeyDriver* gpioDriver = new GPIOKeyDriver();
	SysfsGenericGPIODriver* sysfsDriver = new SysfsGenericGPIODriver();
	GPIOLedDriver* ledDriver = new GPIOLedDriver();
	ADCDriver* adcDriver = new ADCDriver();
	UltraHeatDriver* ultraHeatDriver = new UltraHeatDriver();

	RaspiGPInputDriver* raspiGPInDriver = new RaspiGPInputDriver();
	RaspiGPOutputDriver* raspiGPOutDriver = new RaspiGPOutputDriver();

	SysfsInputDriver* sysfsInput = new SysfsInputDriver();
	SysfsOutputDriver* sysfsOutput = new SysfsOutputDriver();

	//ZWaveSwitchDriver* switchDriver = new ZWaveSwitchDriver();
	FeedSocketDriver* feed = new FeedSocketDriver();

	drivers.push_back( gpioDriver );
	drivers.push_back( sysfsDriver );
	drivers.push_back( ledDriver );
	drivers.push_back( ultraHeatDriver );
	drivers.push_back( adcDriver );
	drivers.push_back( raspiGPInDriver );
	drivers.push_back( raspiGPOutDriver );

	drivers.push_back( sysfsInput );
	drivers.push_back( sysfsOutput );

	//drivers.push_back( switchDriver );
	drivers.push_back( feed );

	EventLogger::Instance()->WriteDebug( "CreateDrivers completed successfully!" );

	return drivers;
}
