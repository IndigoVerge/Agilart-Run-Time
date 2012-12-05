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
#ifndef GPIODEVICE_H_
#define GPIODEVICE_H_

#include "BaseDevice.h"
#include "OutConnectionPoint.h"
#include "InConnectionPoint.h"
#include "BaseDriver.h"
#include "Consts.h"
#include "Enums.h"
#include "Event.h"
#include "EventParameter.h"

class GPOutputDevice: public BaseDevice
{

	public:
		GPOutputDevice();
		virtual ~GPOutputDevice();

		void SetPortNumber( int );
		virtual void ExecuteCommand( Command );

		virtual const char* Type();
		virtual const char* Version();
		virtual const char* Description();
		virtual const char* GroupName();

		virtual BaseDevice* CreateInstance();
		virtual void ReleaseResources();

		DeviceParameter GetParameter( string parameterName );
		virtual void SetParameter( string parameterName, void* value );

	protected:
		virtual void EventTriggered( std::list<EventParameter> );
		virtual void CalculateOutputs();
		virtual void WidgetInitialized();

	private:
		int portNumber;
		/*
		 * The last received parameters. We need them because we need to do recalculation in two cases
		 * 1 - some Event is triggered (someone pressed a button) - we recieve the parameters
		 * 2 - a logical input is changed (IN)
		 */
		//list<EventParameter> stateParameters;
		int isOn;
		InConnectionPoint* inputConnPoint;

};

#endif /* GPIODEVICE_H_ */
