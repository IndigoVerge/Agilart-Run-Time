/*
 * CosmDevice.h
 *
 *  Created on: Sep 20, 2012
 *      Author: developer
 */

#ifndef COSMDEVICE_H_
#define COSMDEVICE_H_

#include "BaseDevice.h"
#include "OutConnectionPoint.h"
#include "InConnectionPoint.h"
#include "BaseDriver.h"
#include "Consts.h"
#include "Enums.h"
#include "Event.h"
#include "EventParameter.h"

#include <curl/curl.h>

class CosmDevice: public BaseDevice
{
	public:
		CosmDevice();
		virtual ~CosmDevice();

		virtual const char* Type();
		virtual const char* Version();
		virtual const char* Description();
		virtual const char* GroupName();

		virtual BaseDevice* CreateInstance();

		DeviceParameter GetParameter( string parameterName );
		virtual void SetParameter( string parameterName, void* value );
		int SendCosmNotification(string apikey, string feedId, string datastreamId, string value);

	protected:
		virtual void CalculateOutputs();

	private:
		int value;
		InConnectionPoint* inputConnPoint;

		string apikey;
		string feedId;
		string datastreamId;

		CURL *curl;
		CURLcode res;

		struct curl_slist *headers;
};

#endif /* COSMDEVICE_H_ */
