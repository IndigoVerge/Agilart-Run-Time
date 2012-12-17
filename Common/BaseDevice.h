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

#ifndef BASEDEVICE_H_
#define BASEDEVICE_H_

#include <list>
#include <map>
#include <stdlib.h>

#include "Pinout.h"
#include "ConnectionPoint.h"
#include "EventParameter.h"
#include "UPCBDescriptors.h"
#include "DeviceParameter.h"

#include "OutConnectionPoint.h"
#include "InConnectionPoint.h"
#include "BaseDriver.h"
#include "Consts.h"
#include "Enums.h"
#include "Event.h"
#include "Property.h"
#include "EventParameter.h"
#include "Logging/EventLogger.h"

#include "ErrorItem.h"
#include "IErrorHandler.h"

class BaseDevice : public IValueChanged, public IRefreshValue
{
	private:
		int id;

		bool isInitialized;
		bool isCustomDevice;
		bool isUpdating;

		string name;
		string groupName;
		string description;

		IErrorHandler* errorsHandler;

	protected:
		DeviceStatus status;
		std::list<Pinout*>* pinouts;
		std::list<ConnectionPoint*>* connectionPoints;
		std::list<DeviceParameter*>* properties;

		friend void PinoutEventTriggered( void* sender, std::list<EventParameter> parameters );

		virtual void EventTriggered( std::list<EventParameter> );
		bool AreAllINsInitialized();

		virtual void WidgetInitialized();

		virtual void CalculateOutputs() = 0;

		void (IErrorHandler::*ErrorHandler)(void* sender, ErrorItem* error);
		virtual void OnError( int errorId );
		virtual void OnError( ErrorItem* error );

	public:
		BaseDevice();
		virtual ~BaseDevice();

		int Id();
		void SetId(int value);

		bool IsInitialized();
		virtual const char* Type() = 0;
		virtual const char* Version() = 0;
		virtual const char* Name();
		virtual const char* Description();
		virtual const char* GroupName();

		void SetName( string );
		void SetIsCustom( bool );
		void SetIsUpdating( bool );

		bool GetIsCustom();
		bool GetIsUpdating();

		std::list<Pinout*>* Pinouts();
		std::list<ConnectionPoint*> ConnectionPoints();

		virtual void ReleaseResources();

		/*
		 * This call is triggered by a Wire which has a change in its state.
		 * The value we get as a parameter is the input connection point which has been just
		 * updated to a new value.
		 *
		 * Here we have to assure that all widget IN pins are up to date and Calculate the widget outputs
		 */
		virtual void InputPinValueChanged( void* connPointPtr );

		/*
		 * This call is triggered by a Wire which wants to recalculate its state. To do so,
		 * the wire needs the latest value of its one and only OUT pin. Therefore it asks the device
		 * to recalculate its output pin. We pass as a parameter the output connection point
		 * which needs to be evaluated.
		 *
		 * Again, here we have to assure that all widget IN pins are up to date and Calculate the widget outputs
		 */
		virtual void Refresh(void* connPointPtr); // OUT

		/*
		 * Used by the Pluggable Device Provider, where we create widgets without knowing their exact types.
		 * To do this, we use the CreateInstance .
		 */
		virtual BaseDevice* CreateInstance() = 0;

		ConnectionPoint* GetConnectionPoint(int id);
		virtual void ExecuteCommand( Command );
		string ToJson();

		virtual DeviceParameter GetParameter( string parameterName );
		virtual void SetParameter( string parameterName, void* value );

		void AddCustomProperty( DeviceParameter* property );
		virtual void InitializeFromDescriptor( list<Property> );

		virtual void RegisterErrorHandler( IErrorHandler* handler );
};

typedef std::list<BaseDevice*> create_devices();
void PinoutEventTriggered( void* sender, std::list<EventParameter> parameters );

#endif /* BASEDEVICE_H_ */

