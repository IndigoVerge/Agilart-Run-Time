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

#ifndef SCREENBUTTON_H_
#define SCREENBUTTON_H_

#include "BaseDevice.h"
#include "OutConnectionPoint.h"
#include "InConnectionPoint.h"
#include "BaseDriver.h"
#include "Consts.h"
#include "Enums.h"
#include "Event.h"
#include "EventParameter.h"
#include "UPCBDescriptors.h"
#include "Logging/EventLogger.h"

class ScreenButton : public BaseDevice
{
	public:
		ScreenButton();
		virtual ~ScreenButton();

		virtual const char* Type();
		virtual const char* Version();
		virtual const char* Description();
		virtual const char* GroupName();

		virtual BaseDevice* CreateInstance();

		virtual DeviceParameter GetParameter( string parameterName );
		virtual void SetParameter( string parameterName, void* value );

	protected:
		void CalculateOutputs();

	private:
		OutConnectionPoint* outConnPoint;
		int currentStatus;
};

#endif /* SCREENBUTTON_H_ */
