/*
 * TemperatureDevice.h
 *
 *  Created on: Feb 24, 2013
 *      Author: dev
 */

#ifndef TEMPERATUREDEVICE_H_
#define TEMPERATUREDEVICE_H_

#include "BaseDevice.h"

class TemperatureDevice : public BaseDevice {
public:
	TemperatureDevice();
	virtual ~TemperatureDevice();

	virtual void ExecuteCommand( Command );

	virtual const char* Type();
	virtual const char* Version();
	virtual const char* Description();
	virtual const char* GroupName();

	virtual BaseDevice* CreateInstance();
	virtual void ReleaseResources();

	DeviceParameter GetParameter( string parameterName );

protected:
	virtual void EventTriggered( std::list<EventParameter> );
	virtual void CalculateOutputs();
	virtual void WidgetInitialized();

private:

	/*
	 * The last received parameters. We need them because we need to do recalculation in two cases
	 * 1 - some Event is triggered (someone pressed a button) - we recieve the parameters
	 * 2 - a logical input is changed (IN)
	 */
	// list<EventParameter> stateParameters;
	float currentPinoutStatus;
	OutConnectionPoint* outputConnPoint;
	Pinout* pinout;
};

#endif /* TEMPERATUREDEVICE_H_ */
