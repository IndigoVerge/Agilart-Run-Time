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

#include <list>
#include "BaseDevice.h"
#include "Logging/EventLogger.h"

#include "ButtonDevice/ButtonDevice.h"
#include "GPOutputDevice/GPOutputDevice.h"
#include "TimerDevice/TimerDevice.h"
#include "DelayTimer/DelayTimer.h"
#include "ScreenButton/ScreenButton.h"
#include "Logical/SwitchBlock.h"
#include "Logical/InverseDevice.h"
#include "Logical/CounterDevice.h"
#include "Logical/RSTriggerDevice.h"
#include "GPInputDevice/GPIDevice.h"
#include "ADCDevice/ADCDevice.h"
#include "Logical/AdnWidget.h"
#include "Logical/OrDevice.h"
#include "Logical/ConstantWidget.h"
#include "Logical/ComparatorWidget.h"
#include "Logical/XorDevice.h"

using namespace std;

extern "C" list<BaseDevice*> CreateDevices()
{
	EventLogger::Instance()->WriteDebug( "Executing CreateDevices!" );

	list<BaseDevice*> devices;

	ButtonDevice* buttonDevice = new ButtonDevice();
	ScreenButton* screenButtonDevice = new ScreenButton();
	GPOutputDevice* gpoDevice = new GPOutputDevice();
	GPIDevice* gpiDevice = new GPIDevice();
	TimerDevice* timerDevice = new TimerDevice();
	DelayTimer* delayTimerDevice = new DelayTimer();
	//SwitchBlock* switchBlock = new SwitchBlock();
	RSTriggerDevice* trigger = new RSTriggerDevice();

	InverseDevice* invDevice = new InverseDevice();
	ADCDevice* adcDevice = new ADCDevice();
	AndWidget* andDevice = new AndWidget();
	OrDevice* orDevice = new OrDevice();
	XorDevice* xorDevice = new XorDevice();

	ConstantDevice* constDev = new ConstantDevice();
	ComparatorDevice* compDev = new ComparatorDevice();
	CounterDevice* counterDevice = new CounterDevice();

	devices.push_back( gpiDevice );
	devices.push_back( gpoDevice );
	devices.push_back( adcDevice );

	devices.push_back( buttonDevice );
	devices.push_back( trigger );
	devices.push_back( screenButtonDevice );

	devices.push_back( andDevice );
    devices.push_back( invDevice );
	devices.push_back( orDevice );
	devices.push_back( xorDevice );

	//devices.push_back( switchBlock );
	devices.push_back( timerDevice );
	devices.push_back( delayTimerDevice );

	devices.push_back( compDev );
	devices.push_back( counterDevice );
	devices.push_back( constDev );

	EventLogger::Instance()->WriteDebug( "CreateDevices completed successfully!" );

	return devices;
}
