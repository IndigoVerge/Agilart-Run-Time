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

#ifndef WIRE_H_
#define WIRE_H_

#include <list>
#include <algorithm>

#include "InConnectionPoint.h"
#include "OutConnectionPoint.h"

class Wire : IValueChanged, IRefreshValue
{
	private:

		std::list<ConnectionPoint *>* connectionPoints;

		void SetValue( void*, AgilartDataTypes );

	public:
		Wire();
		virtual ~Wire();

		/*
		 * Called by the OutConnectionPoint when its value is changed.
		 * The Wire is responsible for
		 *  - goes through all attached to it IN connection points (NEW:) and propagates the new value
		 *  - (REMOVED:) calculates Logical OR between the values of the OUT connection points attached
		 *  - (REMOVED:) if the value is different from the current -> calls SetValue to the attached
		 *  IN connection points
		 *
		 *  ? NotifyValueChanged returns the calculated value to the OUT CP caller as well,
		 *  so that the caller is able to check/validate status.
		 */
		void InputPinValueChanged(void* connPointPtr);

		// TODO - Add comment !
		void Refresh(void* connPointPtr);

		std::list<ConnectionPoint *>* ConnectionPoints();

		void Attach(ConnectionPoint *);
		void Detach(ConnectionPoint *);

		WireDescriptor GetDescriptor();
		Wire* Clone();
};

#endif /* WIRE_H_ */

