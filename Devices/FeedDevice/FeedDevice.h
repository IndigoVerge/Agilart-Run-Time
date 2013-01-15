/*
 * FeedDevice.h
 *
 *  Created on: Jan 7, 2013
 *      Author: dev
 */

#ifndef FEEDDEVICE_H_
#define FEEDDEVICE_H_

#include "BaseDevice.h"
#include "OutConnectionPoint.h"
#include "InConnectionPoint.h"
#include "BaseDriver.h"
#include "Consts.h"
#include "Enums.h"
#include "Event.h"
#include "EventParameter.h"

class FeedDevice : public BaseDevice
{
	public:
		FeedDevice();
		virtual ~FeedDevice();

		virtual const char* Type();
		virtual const char* Version();
		virtual const char* Description();
		virtual const char* GroupName();

		virtual BaseDevice* CreateInstance();

		DeviceParameter GetParameter( string parameterName );
		virtual void SetParameter( string parameterName, void* value );
		virtual void ReleaseResources();

	protected:
		virtual void CalculateOutputs();
		virtual void WidgetInitialized();

	private:
		string value;
		InConnectionPoint* inputConnPoint;
		Pinout* pinout;

		void ExecuteCommand( Command command );
};

#endif /* FEEDDEVICE_H_ */
