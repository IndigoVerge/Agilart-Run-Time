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

#include "GPIDevice.h"

#define GPIBUTTON_CLICK "Switch status changed"
#define GPIBUTTON_DEVNAME "Digital Input"
#define GPIBUTTON_DEVTYPE "GPIDevice"
#define GPIBUTTON_VERSION "0.0.1"
#define GPIBUTTON_DESCR "General Purpose Input Widget with a physical pin to which it connects. Works best when it is used together with the GPIO Keys Driver."
#define GPIBUTTON_GROUP "Hardware"

GPIDevice::GPIDevice() :
	BaseDevice()
{
	this->connectionPoints = new list<ConnectionPoint*> ();
	this->pinouts = new std::list<Pinout*>();

	this->outputConnPoint = new OutConnectionPoint( 2, "Output",
			"Button Device Output", BOOL, new bool(false) );

	this->outputConnPoint->RegisterRefreshHandler( this );
	this->connectionPoints->push_back( this->outputConnPoint );

	// Button Pionout definition:
	Command cmdInit( INIT_CMD );
	Command cmdStop( STOP_CMD );

	list<Command>* pinoutSupportedCommands = new list<Command>();
	pinoutSupportedCommands->push_back( cmdInit );
	pinoutSupportedCommands->push_back( cmdStop );

	list<Event>* pinoutSupportedEvents = new list<Event>();
	list<EventParameter>* eventParams = new list<EventParameter>();
	eventParams->push_back( EventParameter( GPIOKEY_CODE_PARAM, 0, "int" ) );
	eventParams->push_back(EventParameter( GPIOKEY_TYPE_PARAM, 0, "int" ) );
	eventParams->push_back( EventParameter( GPIOKEY_VALUE_PARAM, 0, "int" ) );

	Event eventClick( GPIBUTTON_CLICK, eventParams );
	pinoutSupportedEvents->push_back( eventClick );

	Pinout* pinout = new Pinout( 0, "Button Pinout", "Button Device Pinout",
			pinoutSupportedCommands, pinoutSupportedEvents );
	pinout->PinoutEventTriggered = &PinoutEventTriggered;

	this->pinouts->push_back( pinout );
	this->currentPinoutStatus = 0;
	this->SetName( GPIBUTTON_DEVNAME );
	this->isLatched = false;

	this->AddCustomProperty( new DeviceParameter(IS_LATCHED, BOOL, new bool(false), "Enter true or false. When a Digital Input is latched it acts as a pushdown input and retains its value as long as a change in its state is not present.") );
}

GPIDevice::~GPIDevice()
{
	// implemented in the BaseDevice
}

void GPIDevice::WidgetInitialized()
{
	BaseDevice::WidgetInitialized();

	Command c( INIT_CMD );
	this->ExecuteCommand( c );
}

void GPIDevice::ReleaseResources()
{
	Command c( STOP_CMD );
	this->ExecuteCommand( c );
}

void GPIDevice::CalculateOutputs()
{
	void* resultPointer = new int(this->currentPinoutStatus );
	this->outputConnPoint->SetValue( resultPointer );
}

void GPIDevice::EventTriggered( list<EventParameter> parameters )
{
	EventLogger::Instance()->WriteVerbose( "Button Device - %d - EventTriggered", this->Id() );
	int devicePinOut = 1;

	//TODO - go throuh the pinouts and take the supported events and consider only their parameters!
	list<EventParameter>::iterator pIterator;

	string val = GPIOKEY_VALUE_PARAM;

	if( parameters.size() == 0 )
	{
		devicePinOut = 0;
	}
	else
	{
		for ( pIterator = parameters.begin(); pIterator != parameters.end(); pIterator++ )
		{
			if ( strcmp( ( *pIterator ).Key, val.c_str() ) != 0 )
				continue;

			devicePinOut = ( *pIterator ).Value;
			break;
		}
	}

	if( this->isLatched == true )
	{
		if( devicePinOut == 0 )
		{
			this->currentPinoutStatus = !this->currentPinoutStatus;
			void* resultPointer = new int(this->currentPinoutStatus );
			this->outputConnPoint->SetValue( resultPointer );
		}
	}
	else
	{
		this->currentPinoutStatus = devicePinOut;
		void* resultPointer = new int(this->currentPinoutStatus );
		this->outputConnPoint->SetValue( resultPointer );
	}
}

void GPIDevice::ExecuteCommand( Command command )
{
	string logMessage = "Button Device - Executing Command "
			+ (string) command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	list<Pinout*>::iterator i;

	for ( i = this->pinouts->begin(); i != this->pinouts->end(); i++ )
	{
		( *i )->ExecuteCommand( command );
	}
}

const char* GPIDevice::Type()
{
	return GPIBUTTON_DEVTYPE;
}

const char* GPIDevice::Version()
{
	return GPIBUTTON_VERSION;
}

const char* GPIDevice::Description()
{
	return GPIBUTTON_DESCR;
}

const char* GPIDevice::GroupName()
{
	return GPIBUTTON_GROUP;
}

BaseDevice* GPIDevice::CreateInstance()
{
	BaseDevice* device = new GPIDevice();
	return device;
}

DeviceParameter GPIDevice::GetParameter( string parameterName )
{
	if( this->currentPinoutStatus != 0 && this->currentPinoutStatus != 1 )
		return DeviceParameter();

	if( VALUE_KEY == parameterName )
	{
		bool state = Convert::ToBool( new int(this->currentPinoutStatus), INT );
		DeviceParameter result = DeviceParameter( parameterName, BOOL, new bool(state));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the GPI Device",
					parameterName.c_str());
	return DeviceParameter();
}

void GPIDevice::SetParameter( string parameterName, void* value )
{
	if( IS_LATCHED == parameterName )
	{
		this->isLatched = Convert::ToBool( value, STRING );
	}
	else
	{
		EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Delay Timer Device",
				parameterName.c_str());
	}
}
