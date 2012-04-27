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
#ifndef JSONPARAMETERS_H_
#define JSONPARAMETERS_H_

#include <string>
using namespace std;

class JsonParameters {

public:
	static string Name;
	static string DisplayName;
	static string CmdParameters;
	static string Command;
	static string Commands;
	static string Event;
	static string Events;
	static string EventParameters;
	static string Key;
	static string DataType;
	static string Value;
	static string Driver;
	static string Program;
	static string Mapping;

	static string Wires;
	static string Wire;
	static string Devices;
	static string Device;
	static string DeviceType;
	static string PowerWire;
	static string ConnectionPoints;
	static string UpcbWireConnectionPoint;
	static string Id;
	static string ProjectId;
	static string ProgramDescription;

	static string CommandMapping;
	static string EventMapping;
	static string Source;
	static string Target;
	static string Parameters;
	static string DriverType;
	static string PinoutId;
	static string DeviceId;
	static string ConnectionPointId;
	static string ParameterSource;
	static string ParameterTarget;
	static string ProgramId;
	static string ProgramName;
	static string DeviceConnectionPoints;
	static string ConnectionPoint;
	static string ConnectionPointValue;
	static string ConnPointLabel;
	static string Direction;

	static string Pinouts;
	static string Pinout;
	static string Parameter;
	static string Description;
	static string InConnPointDirection;
	static string OutConnPointDirection;
	static string DeviceProperties;
	static string DevicePropertyKey;
	static string DevicePropertyValue;
	static string DevicePropertyType;
	static string DevicePropertyDescription;

	static string CustomDevice;
	static string CustomDeviceName;
	static string CustomDeviceBaseType;
	static string CustomDeviceAddOnProps;

	static string ProgramAutoStart;
	static string ProgramIsRunning;

	static string OperationStatus;
	static string ErrorCode;
	static string ErrorMessage;

	static string SettingName;
	static string SettingDataType;
	static string SettingValue;
};

#endif /* JSONPARAMETERS_H_ */
