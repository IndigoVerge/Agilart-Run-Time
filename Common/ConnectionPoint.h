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

#ifndef CONNECTIONPOINT_H_
#define CONNECTIONPOINT_H_

#include <iostream>
#include <list>
#include <string.h>

#include "Enums.h"
#include "delegate.h"

#include "IValueChanged.h"
#include "IRefreshValue.h"

#include "JsonParameters.h"
#include "Enums.h"
#include "Convert.h"
#include "Logging/EventLogger.h"

using namespace std;

class ConnectionPoint
{
	protected:
		int id;
		string name;
		PinoutDirection direction;
		string description;

		AgilartDataTypes dataType;
		void* currentValue;

		bool isInitialized;
		bool isCalculated;

		PinState state;

		// NOT USED IValueChanged* target;
		// NOT USED int (IValueChanged::*fptr)();

	public:
		ConnectionPoint( int id, string name, PinoutDirection direction,
				string description, string dataType, void* currentValue );
		ConnectionPoint( int id, string name, PinoutDirection direction,
				string description, AgilartDataTypes dataType, void* currentValue );
		virtual ~ConnectionPoint();

		int Id();
		std::string Name();
		PinoutDirection Direction();
		std::string Description();
		void* Value();
		AgilartDataTypes DataType();

		bool IsInitialized();
		bool IsCalculated(); // used in the initialization phase

		void ConnectionPointInitialized();

		void SetDataType( AgilartDataTypes );
		void SetIsInitialized( bool );
		void SetIsCalculated( bool );

		virtual void SetValue( void* );

		virtual ConnectionPoint* Clone();

		/* IN:
		 * The value in the connection point has changes. Notify the devise so
		 * that it can apply its logic calculate a new value and
		 * eventually set its OUT connection points to the new value
		 * OUT:
		 * Implemented by the wire to which this ConnectionPoint is attached.
		 * ValueChanged here triggers NotifyValueChanged in the wire - responsible
		 * for the current wire value recalculation.
		 */
		virtual void RegisterValueChangedHandler( IValueChanged* handler );
		virtual void TriggerValueChanged();

		// TODO - Add coment !
		virtual	void RegisterRefreshHandler( IRefreshValue* handler );
		virtual	void Refresh();

		std::string ToJson();

		PinState GetState();
		void SetState( PinState );
};

#endif /* DEVICEPINOUT_H_ */

