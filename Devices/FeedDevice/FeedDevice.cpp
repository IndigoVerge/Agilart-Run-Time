/*
 * FeedDevice.cpp
 *
 *  Created on: Jan 7, 2013
 *      Author: dev
 */

#include <iostream>
#include "FeedDevice.h"
#include "EventParameter.h"
#include "Logging/EventLogger.h"
#include "Pinout.h"
#include "CommandParameter.h"

#define FEED_DEVNAME "Feed Device"
#define FEED_DEVTYPE "FeedDevice"
#define FEED_VERSION "0.0.1"
#define FEED_DESCR ""
#define FEED_GROUP "Software"

using namespace std;

FeedDevice::FeedDevice() {

	this->connectionPoints = new list<ConnectionPoint*>();
	this->pinouts = new std::list<Pinout*>();

	this->inputConnPoint = new InConnectionPoint( 1, "Input", "Input Value", INT, new int(0) );
	this->inputConnPoint->RegisterValueChangedHandler( this );

	this->connectionPoints->push_back( this->inputConnPoint );

	// Pinout Definition
	Command cmdInit( INIT_CMD );
	Command cmdStop( STOP_CMD );
	Command cmdSetValue( SETVAL_CMD );
	cmdSetValue.AddParameter( new CommandParameter( VALUE_KEY, NULL, STRING_TYPE ) );

	list<Command>* pinoutSupportedCommands = new list<Command>();
	pinoutSupportedCommands->push_back( cmdInit );
	pinoutSupportedCommands->push_back( cmdStop );
	pinoutSupportedCommands->push_back( cmdSetValue );

	list<Event>* pinoutSupportedEvents = new list<Event>();

	this->pinout = new Pinout( 0, "Feed Pinout", "Feed Pinout",
				pinoutSupportedCommands, pinoutSupportedEvents );
	this->pinout->PinoutEventTriggered = &PinoutEventTriggered;
	this->pinouts->push_back( pinout );

	this->SetName( FEED_DEVNAME );
}

FeedDevice::~FeedDevice()
{
	if(this->pinout != NULL)
		delete this->pinout;
}

const char* FeedDevice::Type()
{
	return FEED_DEVTYPE;
}

const char* FeedDevice::Version()
{
	return FEED_VERSION;
}

const char* FeedDevice::Description()
{
	return FEED_DESCR;
}

const char* FeedDevice::GroupName()
{
	return FEED_GROUP;
}

BaseDevice* FeedDevice::CreateInstance()
{
	BaseDevice* device = new FeedDevice();
	return device;
}

DeviceParameter FeedDevice::GetParameter( string parameterName )
{
	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, STRING, new string( this->value ));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Feed Device",
					parameterName.c_str());
	return DeviceParameter();
}

void FeedDevice::WidgetInitialized()
{
	BaseDevice::WidgetInitialized();

	Command c( INIT_CMD );
	this->ExecuteCommand( c );
}

void FeedDevice::ReleaseResources()
{
	Command c( STOP_CMD );
	this->ExecuteCommand( c );
}

void FeedDevice::SetParameter( string parameterName, void* value )
{
}

void FeedDevice::CalculateOutputs()
{
	void* val = this->inputConnPoint->Value();
	string input = Convert::ToString( val, INT );

	CommandParameter* cp = new CommandParameter( VALUE_KEY, (void*)(new string( input )), STRING_TYPE );

	// Send a command to the ( pinout -> MappingManager -> device driver ) to do something - change its state.
	Command c( SETVAL_CMD );
	c.AddParameter( cp );

	this->ExecuteCommand( c );
}

void FeedDevice::ExecuteCommand( Command command )
{
	string logMessage = "Feed Device - Executing Command "
			+ (string) command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );
	this->pinout->ExecuteCommand( command );

}
