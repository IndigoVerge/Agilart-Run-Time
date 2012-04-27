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

#ifndef OUTCONNECTIONPOINT_H_
#define OUTCONNECTIONPOINT_H_

#include "ConnectionPoint.h"
#include "InConnectionPoint.h"
#include "IValueChanged.h"
#include "IRefreshValue.h"

class OutConnectionPoint: public ConnectionPoint
{
	private:
		IValueChanged* handler;
		IRefreshValue* refreshHandler;

	public:
		OutConnectionPoint(int, string, string, string, void* );
		OutConnectionPoint(int, string, string, AgilartDataTypes, void* );
		virtual ~OutConnectionPoint();

		/*
		 * During the initialization steps we go through the devices one after the other.
		 * For each device we want to get all INs calculate the OUT.
		 * For getting INs we fall into two cases:
		 *  - the connection point is initialized - then we get its current value
		 *  - the connection point is not initialized - we have to force the calculation of the current value
		 *
		 * We have to get a LOGIC OR between the OUTs to which this IN is connected.
		 * To get the OUT value we trigger a callback defined in the device which will calculate the OUT value.
		 *
		 */
		// virtual void* GetValue(); - implemented in the base
		/*
		 * Returns true or false depending on whether the value has been changed or not.
		 */
		virtual void SetValue( void* );

		void* (*CalculateValue)();
		ConnectionPoint* Clone();

		void (IValueChanged::*WireValueChanged)(void* connPointPtr);
		virtual void RegisterValueChangedHandler( IValueChanged* handler );
		virtual void TriggerValueChanged();

		void (IRefreshValue::*RefreshValue)(void* connPointPtr);
		virtual	void RegisterRefreshHandler( IRefreshValue* handler );
		virtual	void Refresh();
};

#endif /* OUTCONNECTIONPOINT_H_ */

