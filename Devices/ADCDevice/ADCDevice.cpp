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

#include "ADCDevice.h"

#define ADCDevice_DEVNAME "ADC Device"
#define ADCDevice_DEVTYPE "GPIDevice"
#define ADCDevice_VERSION "0.0.1"
#define ADCDevice_DESCR "Analog to Digital Values Converter Widget"
#define ADCDevice_GROUP "Hardware"

#define ADCDevice_VALUE "Value"
#define ADCDevice_VALEVENT "Value Changed"

ADCDevice::ADCDevice() :
	BaseDevice()
{
	this->connectionPoints = new list<ConnectionPoint*> ();
	this->pinouts = new std::list<Pinout*>();

	this->outPoint = new OutConnectionPoint( 2, "Output",
			"ADC Device Output", FLOAT_TYPE, new float(0) );

	this->outPoint->RegisterRefreshHandler( this );
	this->connectionPoints->push_back( outPoint );

	// ADC Pionout definition:
	Command cmdInit( INIT_CMD );
	Command cmdStop( STOP_CMD );

	list<Command>* pinoutSupportedCommands = new list<Command>();
	pinoutSupportedCommands->push_back( cmdInit );
	pinoutSupportedCommands->push_back( cmdStop );

	list<Event>* pinoutSupportedEvents = new list<Event>();
	list<EventParameter>* eventParams = new list<EventParameter>();
	eventParams->push_back( EventParameter( ADCDevice_VALUE, 0, INT_TYPE ) );

	Event eventClick( ADCDevice_VALEVENT, eventParams );
	pinoutSupportedEvents->push_back( eventClick );

	Pinout* pinout = new Pinout( 0, "ADC Pinout", "ADC Device Pinout",
			pinoutSupportedCommands, pinoutSupportedEvents );
	pinout->PinoutEventTriggered = &PinoutEventTriggered;

	this->pinouts->push_back( pinout );
	this->currentPinoutValue = 0.0f;
	this->SetName( ADCDevice_DEVNAME );
}

ADCDevice::~ADCDevice()
{
	// implemented in the BaseDevice
}

void ADCDevice::WidgetInitialized()
{
	Command c( INIT_CMD );
	this->ExecuteCommand( c );
}

void ADCDevice::ReleaseResources()
{
	Command c( STOP_CMD );
	this->ExecuteCommand( c );
}

void ADCDevice::CalculateOutputs()
{
	EventLogger::Instance()->WriteVerbose(
			"ADC Device(%d) - LogicalInValueChanged", this->Id() );

	void* resultPointer = new float( this->currentPinoutValue );
	this->outPoint->SetValue( resultPointer );
}

void ADCDevice::EventTriggered( list<EventParameter> parameters )
{
	EventLogger::Instance()->WriteVerbose( "ADC Device - %d - EventTriggered", this->Id() );

	int devicePinOut = -1;
	list<EventParameter>::iterator pIterator;
	string val = ADCDevice_VALUE;

	if( parameters.size() != 0 )
	{
		for ( pIterator = parameters.begin(); pIterator != parameters.end(); pIterator++ )
		{
			if ( strcmp( ( *pIterator ).Key, val.c_str() ) == 1 )
				continue;

			devicePinOut = ( *pIterator ).Value;
		}
	}

	this->currentPinoutValue = devicePinOut;
	void* resultPointer = new float( this->currentPinoutValue );

	this->outPoint->SetValue( resultPointer );
}

void ADCDevice::ExecuteCommand( Command command )
{
	string logMessage = "ADC Device - Executing Command "
			+ (string) command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	list<Pinout*>::iterator i;

	for ( i = this->pinouts->begin(); i != this->pinouts->end(); i++ )
	{
		( *i )->ExecuteCommand( command );
	}
}

const char* ADCDevice::Type()
{
	return ADCDevice_DEVTYPE;
}

const char* ADCDevice::Version()
{
	return ADCDevice_VERSION;
}

const char* ADCDevice::Description()
{
	return ADCDevice_DESCR;
}

const char* ADCDevice::GroupName()
{
	return ADCDevice_GROUP;
}

BaseDevice* ADCDevice::CreateInstance()
{
	BaseDevice* device = new ADCDevice();
	return device;
}

DeviceParameter ADCDevice::GetParameter( string parameterName )
{
	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, FLOAT, new float( this->currentPinoutValue ));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the ADC Device",
					parameterName.c_str());
	return DeviceParameter();
}
