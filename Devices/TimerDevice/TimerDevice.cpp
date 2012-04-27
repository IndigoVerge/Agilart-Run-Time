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

#include "TimerDevice.h"
#define TIMER_INTERVAL "Timer Interval"
#define TIMER_DEFAULT_INTERVAL "10" // In seconds
#define INT_TYPE "int"

#define RETTM_DEVNAME "Retentive Timer"
#define RETTM_DEVTYPE "LogicalDevice"
#define RETTM_VERSION "0.0.1"
#define RETTM_DESCR "Retentive Timer"
#define RETTM_GROUP "Other"

TimerDevice::TimerDevice() :
	BaseDevice()
{
	this->interval = 10;
	this->timeLeft = 0;
	this->shouldLoopAndReset = false;
	this->shouldStop = false;
	this->shouldRetainTimerValue = false;

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

	this->AddCustomProperty( new DeviceParameter(TIMER_INTERVAL, INT_TYPE, TIMER_DEFAULT_INTERVAL, "The amount of time in seconds, the timer should wait before reseting the output value to its initial state.") );
	this->SetName(RETTM_DEVNAME);

	int returnStatus = pthread_mutex_init(&this->commonResourcesMutex, NULL);

	if (returnStatus != 0) {
		EventLogger::Instance()->WriteVerbose( ((string)"Error creating Timer Device Common Resources Mutex!").c_str() );
	}
}

TimerDevice::~TimerDevice() {
	// implemented in the base
}

void TimerDevice::ReleaseResources() {
	this->StopThread();
}

void TimerDevice::CalculateOutputs()
{
	if( this->shouldStop == true )
		return;

	if( this->IsInitialized()  == false )
	{
		void* resultPointer = new int(0);
		this->outConnPoint->SetValue(resultPointer);
		return;
	}

	EventLogger::Instance()->WriteVerbose(
			"Timer Device - %d - LogicalInValueChanged", this->Id());

	int inValue = *(int*) this->inConnPoint->Value();
	void* resultPointer = new int(inValue);

	if (inValue == 1)
	{
		this->outConnPoint->SetValue(resultPointer);

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
	else if (inValue == 0 && this->status == DEVICE_INITIALIZING)
	{
		this->outConnPoint->SetValue(resultPointer);
	}
}

void TimerDevice::WidgetInitialized()
{
	BaseDevice::WidgetInitialized();
	this->CalculateOutputs();
}

void* TimerThreadRun(void* param) {
	TimerDevice* device = (TimerDevice*) param;

	pthread_mutex_lock(&device->commonResourcesMutex);
	device->isThreadRunning = true;
	pthread_mutex_unlock(&device->commonResourcesMutex);

	clock_t endwait;

	endwait = clock() + device->interval * CLOCKS_PER_SEC;
	while (clock() < endwait) {
		device->timeLeft = (int)((endwait - clock()) / CLOCKS_PER_SEC);
		// wait
	}

	bool isProgramRunning = true;
	pthread_mutex_lock(&device->commonResourcesMutex);
	isProgramRunning = !(device->shouldStop);
	pthread_mutex_unlock(&device->commonResourcesMutex);

	if( isProgramRunning == true )
	{
		void* zeroPointer = new int(0);
		EventLogger::Instance()->WriteVerbose( "Set Value in %d - 0", device->Id() );
		device->outConnPoint->SetValue(zeroPointer);
	}

	pthread_mutex_lock(&device->commonResourcesMutex);
	device->isThreadRunning = false;
	pthread_mutex_unlock(&device->commonResourcesMutex);

	pthread_exit((void *) 0);
}

void TimerDevice::StartThread() {

	int returnStatus = pthread_create(&this->timerThread, NULL, TimerThreadRun, this);

	if (returnStatus != 0) {
		EventLogger::Instance()->WriteError( ((string) "Error creating timer Device thread!").c_str() );
	}
}

void TimerDevice::StopThread() {
	pthread_mutex_lock(&this->commonResourcesMutex);
	this->shouldStop = true;
	pthread_mutex_unlock(&this->commonResourcesMutex);

	EventLogger::Instance()->WriteInformation( "Stopping Timer Thread." );
}

const char* TimerDevice::Type() {
	return RETTM_DEVTYPE;
}

const char* TimerDevice::Version() {
	return RETTM_VERSION;
}

const char* TimerDevice::Description() {
	return RETTM_DESCR;
}

const char* TimerDevice::GroupName() {
	return RETTM_GROUP;
}

BaseDevice* TimerDevice::CreateInstance() {
	BaseDevice* device = new TimerDevice();
	return device;
}

void TimerDevice::InitializeFromDescriptor(list<PropertyDescriptor> properties) {
	list<PropertyDescriptor>::iterator it;

	for (it = properties.begin(); it != properties.end(); it++) {
		if ((*it).Name == TIMER_INTERVAL) {
			this->interval = atoi((*it).Value.c_str());
			break;
		}
	}
}

DeviceParameter TimerDevice::GetParameter( string parameterName )
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

void TimerDevice::SetParameter( string parameterName, void* value )
{
	if( TIMER_INTERVAL == parameterName )
	{
		string stringValue = *(string*)value;
		this->interval = atoi(stringValue.c_str());
	}
	else
	{
		EventLogger::Instance()->WriteVerbose("Parameter with name %s in to recognized by the Retentive Timer Device",
				parameterName.c_str());
	}
}
