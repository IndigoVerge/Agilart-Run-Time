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
#include <iostream>

#include "CosmDevice.h"
#include "EventParameter.h"
#include "Logging/EventLogger.h"

#define COSM_DEVNAME "COSM"
#define COSM_DEVTYPE "COSMDevice"
#define COSM_VERSION "0.0.1"
#define COSM_DESCR "Cosm is a secure, scalable platform that connects devices and products with applications to provide real-time control and data storage. Using Cosm's open API, individuals and companies can create new devices, develop prototypes, and bring products to market in volume."
#define COSM_GROUP "Software"
#define COSM_DEFAULT_FEEDID "76561"
#define COSM_FEEDID "Cosm Feed Id"
#define COSM_APIKEY "Cosm API Key"
#define COSM_DEFAULT_APIKEY "MrzBHvcIMa-Kf4w3v878TjhynriSAKxEQWhGYXNBOXEzZz0g"
#define COSM_DATASTREAM "Datastream Id"
#define COSM_DEFAULT_DATASTREAM "Default Datastream Id"

using namespace std;

CosmDevice::CosmDevice() :
	BaseDevice()
{
	this->value = 0;
	this->connectionPoints = new list<ConnectionPoint*>();
	this->pinouts = new std::list<Pinout*>();

	this->inputConnPoint = new InConnectionPoint( 1, "Input", "Input Value", INT, new int(0) );
	this->inputConnPoint->RegisterValueChangedHandler( this );

	this->connectionPoints->push_back( this->inputConnPoint );

	this->AddCustomProperty( new DeviceParameter(COSM_FEEDID, STRING_TYPE, COSM_DEFAULT_FEEDID, "Your COSM Feed Id") );
	this->AddCustomProperty( new DeviceParameter(COSM_APIKEY, STRING_TYPE, COSM_DEFAULT_APIKEY, "Your COSM API Key") );
	this->AddCustomProperty( new DeviceParameter(COSM_DATASTREAM, STRING_TYPE, COSM_DEFAULT_DATASTREAM, "Datastream Id") );
	// ? data stream type

	this->SetName( COSM_DEVNAME );

    this->curl = curl_easy_init();
	this->headers = NULL;
}

CosmDevice::~CosmDevice()
{
	// implemented in the BaseDevice


    /* always cleanup */
    curl_easy_cleanup(curl);

    /* free slist */
    curl_slist_free_all (headers);
}

void CosmDevice::CalculateOutputs()
{
	void* val = this->inputConnPoint->Value();
	string input = Convert::ToString( val, INT );

	this->SendCosmNotification( this->apikey, this->feedId, this->datastreamId, input );
}

const char* CosmDevice::Type()
{
	return COSM_DEVTYPE;
}

const char* CosmDevice::Version()
{
	return COSM_VERSION;
}

const char* CosmDevice::Description()
{
	return COSM_DESCR;
}

const char* CosmDevice::GroupName()
{
	return COSM_GROUP;
}

BaseDevice* CosmDevice::CreateInstance()
{
	BaseDevice* device = new CosmDevice();
	return device;
}

DeviceParameter CosmDevice::GetParameter( string parameterName )
{
	if( this->value != 0 && this->value != 1 )
		return DeviceParameter();

	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, INT, new int( this->value ));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Cosm Device",
					parameterName.c_str());
	return DeviceParameter();
}

void CosmDevice::SetParameter( string parameterName, void* value )
{
	if( COSM_APIKEY == parameterName )
	{
		this->apikey = Convert::ToString( value, STRING );
		this->headers = curl_slist_append(headers, ("X-ApiKey: " + apikey).c_str());
	}
	else if( COSM_FEEDID == parameterName )
	{
		this->feedId = Convert::ToString( value, STRING );
	}
	else if( COSM_DATASTREAM == parameterName )
	{
		this->datastreamId = Convert::ToString( value, STRING );
	}
	else
	{
		EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Cosm Device",
				parameterName.c_str());
	}
}

int CosmDevice::SendCosmNotification(string apikey, string feedId, string datastreamId, string value)
{
	    if(curl) {

	     string val = "{ \"current_value\":\"" + value + "\"}";
		 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, val.c_str());
		 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		 string url = "api.cosm.com/v2/feeds/" + feedId + "/datastreams/" + datastreamId + "?_method=put";
		 curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	      /* Perform the request, res will get the return code */
	      res = curl_easy_perform(curl);
	      /* Check for errors */
	      if(res != CURLE_OK)
	        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));


		  EventLogger::Instance()->WriteVerbose("cosm: %s : %s", url.c_str(), val.c_str());


	    }
	    return 0;
}
