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

#include "DelayTimer.h"
#define TIMER_INTERVAL "Interval"
#define TIMER_DEFAULT_INTERVAL "10" // In seconds
#define INT_TYPE "int"
#define TIMER_DEVNAME "Delay Timer"
#define TIMER_DEVTYPE "LogicalDevice"
#define TIMER_VERSION "0.0.1"
#define TIMER_DESCR "Timer On Delay - Delays the signal with a specific amount of seconds"
#define TIMER_GROUP "Other"

DelayTimer::DelayTimer() :
	BaseDevice()
{
	this->interval = 10;
	this->shouldLoopAndReset = false;
	this->shouldStop = false;
	this->shouldBreakLoop = false;

	this->connectionPoints = new list<ConnectionPoint*> ();
	this->pinouts = new std::list<Pinout*>();

	this->inConnPoint = new InConnectionPoint(1, "Input", "Timer Device Input",
			"int", new int(0));

	this->inConnPoint->RegisterValueChangedHandler( this );
	this->connectionPoints->push_back(this->inConnPoint);

	this->outConnPoint = new OutConnectionPoint(2, "Output",
			"Timer Device Output", "int", new int(0));
	this->outConnPoint->RegisterRefreshHandler( this );
	this->connectionPoints->push_back(this->outConnPoint);

	this->AddCustomProperty( new DeviceParameter(TIMER_INTERVAL, INT_TYPE, TIMER_DEFAULT_INTERVAL, "The amount of time in seconds, the timer should delay the output value.") );
	this->SetName(TIMER_DEVNAME);

	this->timeLeft = 0;
	this->isThreadRunning = false;

	int returnStatus = pthread_mutex_init(&this->commonResourcesMutex, NULL);
	if (returnStatus != 0) {
		EventLogger::Instance()->WriteVerbose( ((string)"Error creating Timer Device Common Resources Mutex!").c_str() );
	}
}

DelayTimer::~DelayTimer() {
	// TODO Auto-generated destructor stub
}

void DelayTimer::CalculateOutputs()
{
	int inValue = *(int*) this->inConnPoint->Value();

	if (inValue == 1)
	{
		pthread_mutex_lock(&this->commonResourcesMutex);
		if( this->isThreadRunning == false )
		{
			pthread_mutex_unlock(&this->commonResourcesMutex);
			this->StartThread();
		}
		else
		{
			pthread_mutex_unlock(&this->commonResourcesMutex);
		}
	}
	else if ( inValue == 0 && this->status == DEVICE_INITIALIZING )
	{
		void* resultPointer = new int(inValue);
		this->outConnPoint->SetValue(resultPointer);
	}
}

void* DelayTimerThreadRun(void* param) {
	DelayTimer* device = (DelayTimer*) param;

	clock_t endwait;

	pthread_mutex_lock(&device->commonResourcesMutex);
	endwait = clock() + device->interval * CLOCKS_PER_SEC;
	pthread_mutex_unlock(&device->commonResourcesMutex);

	while (clock() < endwait) {
		//wait
		device->timeLeft = (int)((endwait - clock()) / CLOCKS_PER_SEC);
	}

	void* onePointer = new int(1);
	device->outConnPoint->SetValue( onePointer );

	void* zeroPointer = new int(0);
	device->outConnPoint->SetValue( zeroPointer );

	pthread_exit((void *) 0);
}

void DelayTimer::StartThread() {
	int returnStatus = pthread_create(&this->timerThread, NULL, DelayTimerThreadRun,
			this);

	if (returnStatus != 0) {
		EventLogger::Instance()->WriteVerbose( ((string)"Error creating timer Device thread!").c_str() );
	}
}

void DelayTimer::StopThread() {
	pthread_mutex_lock(&this->commonResourcesMutex);
	this->shouldStop = true;
	pthread_mutex_unlock(&this->commonResourcesMutex);
}

const char* DelayTimer::Type() {
	return TIMER_DEVTYPE;
}

const char* DelayTimer::Version() {
	return TIMER_VERSION;
}

const char* DelayTimer::Description() {
	return TIMER_DESCR;
}

const char* DelayTimer::GroupName() {
	return TIMER_GROUP;
}

BaseDevice* DelayTimer::CreateInstance() {
	BaseDevice* device = new DelayTimer();
	return device;
}

void DelayTimer::InitializeFromDescriptor(list<Property> properties) {
	list<Property>::iterator it;

	for (it = properties.begin(); it != properties.end(); it++) {
		if ((*it).Name == TIMER_INTERVAL) {
			this->interval = atoi((*it).Value.c_str());
			break;
		}
	}
}

DeviceParameter DelayTimer::GetParameter( string parameterName )
{
	if( VALUE_KEY == parameterName )
	{
		DeviceParameter result = DeviceParameter( parameterName, INT, new int( this->timeLeft ));
		return result;
	}

	EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Timer Device",
					parameterName.c_str());

	return DeviceParameter();
}

void DelayTimer::SetParameter( string parameterName, void* value )
{

	if( TIMER_INTERVAL == parameterName )
	{
		string stringValue = *(string*)value;
		this->interval = atoi(stringValue.c_str());
	}
	else
	{
		EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Delay Timer Device",
				parameterName.c_str());
	}
}
