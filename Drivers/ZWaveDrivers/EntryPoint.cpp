/*
 * EntryPoint.cpp
 *
 *  Created on: Dec 28, 2012
 *      Author: dev
 */

#include <list>
#include "BaseDriver.h"
#include "Logging/EventLogger.h"

#include "ZSwtichDriver.h"


using namespace std;

extern "C" list<BaseDriver*> CreateDrivers()
{
	EventLogger::Instance()->WriteDebug( "Executing ZWave CreateDrivers!" );

	list<BaseDriver*> drivers;

	ZSwtichDriver* switchDriver = new ZSwtichDriver();
	drivers.push_back( switchDriver );

	EventLogger::Instance()->WriteDebug( "ZWave CreateDrivers completed successfully!" );

	return drivers;
}

