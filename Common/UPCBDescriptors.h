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

#ifndef UPCBDESCRIPTORS_H_
#define UPCBDESCRIPTORS_H_

#include <list>
/*
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
*/
using namespace std;
//using boost::serialization::make_nvp;
/*
class PropertyDescriptor {
public:
	string Name;
	string DataType;
	string Value;

	PropertyDescriptor()
	{

	}

	PropertyDescriptor(string name, string dataType, string value) {
		this->Name = name;
		this->DataType = dataType;
		this->Value = value;
	}

	~PropertyDescriptor() {
	}

};

class ConnectionPointDescriptor {
public:
	list<PropertyDescriptor> Properties;

	ConnectionPointDescriptor()
	{

	}

	~ConnectionPointDescriptor()
	{

	}


};

class CommandParameterDescriptor {
public:
	list<PropertyDescriptor> Properties;

};

class CommandDescriptor {
public:
	list<PropertyDescriptor> Properties;
	list<CommandParameterDescriptor> CommandParameters;

};

class EventParameterDescriptor {
public:
	list<PropertyDescriptor> Properties;

};

class EventDescriptor {
public:
	list<PropertyDescriptor> Properties;
	list<EventParameterDescriptor> EventParameters;

};

class PinoutDescriptor {
public:
	list<PropertyDescriptor> Properties;
	list<CommandDescriptor> SupportedCommands;
	list<EventDescriptor> SupportedEvents;

};

class DeviceDescriptor {
public:
	list<PropertyDescriptor> Properties;
	list<ConnectionPointDescriptor> ConnectionPoints;
	list<PinoutDescriptor> Pinouts;

};

class DriverDescriptor {
public:
	list<PropertyDescriptor> Properties;
	list<CommandDescriptor> SupportedCommands;
	list<EventDescriptor> SupportedEvents;

};

class WireConnectionPointDescriptor {
public:
	int DeviceDescriptorId;
	int ConnectionPointDescriptorId;

};

class WireDescriptor {
public:
	list<PropertyDescriptor> Properties;
	list<WireConnectionPointDescriptor> WireConnectionPoints;

};

class ProgramDescriptor {
public:
	list<PropertyDescriptor> Properties;
	list<DeviceDescriptor> Devices;
	list<WireDescriptor> Wires;

	WireDescriptor PowerWire;

	string Name()
	{
		string key = "Name";
		string name;
		list<PropertyDescriptor>::iterator it;

		for ( it = Properties.begin(); it != Properties.end(); it++ )
		{
			if ( key == (*it).Name )
			{
				name = (*it).Value;
				break;
			}
		}

		return name;
	}

};

class PinoutMappingDescriptor {
public:
	list<PropertyDescriptor> Properties;
	list<CommandDescriptor> CommandMappings;
	list<EventDescriptor> EventMappings;

};

class ProgramMappingDescriptor {
public:
	list<PinoutMappingDescriptor> PinoutMappings;

}; */

#endif /* UPCBDESCRIPTORS_H_ */
