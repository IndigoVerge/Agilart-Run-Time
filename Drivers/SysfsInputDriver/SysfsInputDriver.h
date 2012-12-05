/*
 * SysfsInputDriver.h
 *
 *  Created on: Nov 27, 2012
 *      Author: developer
 */

#ifndef SYSFSINPUTDRIVER_H_
#define SYSFSINPUTDRIVER_H_

#include <list>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "BaseDriver.h"
#include "Command.h"
#include "CommandParameter.h"
#include "Consts.h"
#include "Logging/EventLogger.h"

class SysfsInputDriver : public BaseDriver
{

public:
	SysfsInputDriver();
	~SysfsInputDriver();

	virtual const char* Type();
	virtual const char* Version();
	virtual const char* Name();
	virtual const char* Description();
	virtual const char* Publisher();

	virtual void ExecuteCommand( Command );
	virtual Command* GetCommand( const char* );
	virtual std::list<Command> Commands();
	virtual BaseDriver* Clone();

	int GetStatus();
	void SetStatus(int value);
	int GetPortNumber();
	int GetGpioValue();
	void SignalPortstatusChanged();
	int gpioFD;

	friend void* SysfsInputThreadRun( void* param );

private:
	int portNumber;
	int status;
	bool shouldStop;

	pthread_t refreshStatusThread;
	pthread_mutex_t commonResourcesMutex;

	void StartPoolingForStatus();
	void StopPoolingForStatus();

	virtual int OpenPort( int, int );
	virtual int ClosePort( int );

	virtual void SetValue( int, int );
	virtual int GetValue( int );

	int gpio_export(unsigned int);
	int gpio_unexport(unsigned int);
	int gpio_set_dir(unsigned int, unsigned int);
	int gpio_set_value(int, int);
	int gpio_get_value(int, int*);
	int gpio_set_edge(unsigned int, char*);
	int gpio_fd_open(unsigned int);
	int gpio_fd_close(int);
};

#endif /* SYSFSINPUTDRIVER_H_ */
