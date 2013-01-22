/*
 * TemperatureDriver.h
 *
 *  Created on: Feb 24, 2013
 *      Author: dev
 */

#ifndef TEMPERATUREDRIVER_H_
#define TEMPERATUREDRIVER_H_

#include "BaseDriver.h"

class TemperatureDriver : public BaseDriver
{
public:
	TemperatureDriver();
	virtual ~TemperatureDriver();

	virtual const char* Type();
	virtual const char* Version();
	virtual const char* Name();
	virtual const char* Description();
	virtual const char* Publisher();

	virtual void ExecuteCommand( Command );
	virtual Command* GetCommand( const char* );
	virtual std::list<Command> Commands();
	virtual BaseDriver* Clone();

	float ReadValue();
	void SetCurrentValue(float newValue);

	friend void* ReadTempertureThreadRun( void* param );

private:
	float currentValue;
	bool shouldStop;
	int fd;
	int refreshOn;
	string address;
	string devName;

	pthread_t readKeyThread;
	pthread_mutex_t commonResourcesMutex;

	int OpenPort();
	void ClosePort();

	void StartReadingThread();
	void StopReadingThread();
};

#endif /* TEMPERATUREDRIVER_H_ */
