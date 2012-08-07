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

#include "DescriptorsUtil.h"

using namespace std;

DescriptorsUtil::DescriptorsUtil()
{
	// TODO Auto-generated constructor stub

}

DescriptorsUtil::~DescriptorsUtil()
{
	// TODO Auto-generated destructor stub
}

bool DescriptorsUtil::GetStringProperty( list<PropertyDescriptor> properties,
		string key, string *result )
{
	bool isFound = false;
	list<PropertyDescriptor>::iterator it;

	for ( it = properties.begin(); it != properties.end(); it++ )
	{
		if ( key == ( *it ).Name )
		{
			*result = ( *it ).Value;
			isFound = true;
			break;
		}
	}

	return isFound;
}

bool DescriptorsUtil::GetIntProperty( list<PropertyDescriptor> properties,
		string key, int *result )
{
	string strngResult;
	bool isFound = GetStringProperty( properties, key, &strngResult );

	if ( isFound == false )
		return false;

	*result = atoi( strngResult.c_str() );

	return true;
}

ns1__SoapProgramDescriptor* DescriptorsUtil::GetSoapProgramDescriptor( ProgramDescriptor program )
{
	ns1__SoapProgramDescriptor* soapProgram = new ns1__SoapProgramDescriptor();

	// CLEAN UP !
	soapProgram->Devices = new ns1__ArrayOfSoapDeviceDescriptor();
	soapProgram->Properties = new ns1__ArrayOfSoapPropertyDescriptor();
	soapProgram->Wires = new ns1__ArrayOfSoapWireDescriptor();
	soapProgram->PowerWire = DescriptorsUtil::GetSoapWireDescriptor( program.PowerWire );

	list<PropertyDescriptor>::iterator propIt;
	list<DeviceDescriptor>::iterator deviceIt;
	list<WireDescriptor>::iterator wireIt;

	for( propIt = program.Properties.begin(); propIt != program.Properties.end(); propIt++ )
	{
		soapProgram->Properties->SoapPropertyDescriptor.push_back(
				DescriptorsUtil::GetSoapPropertyDescriptor( *propIt ) );
	}

	for( deviceIt = program.Devices.begin(); deviceIt != program.Devices.end(); deviceIt++ )
	{
		soapProgram->Devices->SoapDeviceDescriptor.push_back(
				DescriptorsUtil::GetSoapDeviceDescriptor( *deviceIt ) );
	}

	for( wireIt = program.Wires.begin(); wireIt != program.Wires.end(); wireIt++ )
	{
		soapProgram->Wires->SoapWireDescriptor.push_back(
				DescriptorsUtil::GetSoapWireDescriptor( *wireIt ) );
	}

	return soapProgram;
}

ns1__SoapProgramDescriptor* DescriptorsUtil::GetSoapProgramStatusDescriptor( ProgramDescriptor program )
{
	ns1__SoapProgramDescriptor* soapProgram = new ns1__SoapProgramDescriptor();

	// CLEAN UP !
	soapProgram->Devices = new ns1__ArrayOfSoapDeviceDescriptor();
	soapProgram->Properties = new ns1__ArrayOfSoapPropertyDescriptor();

	list<PropertyDescriptor>::iterator propIt;
	list<DeviceDescriptor>::iterator deviceIt;

	for( propIt = program.Properties.begin(); propIt != program.Properties.end(); propIt++ )
	{
		soapProgram->Properties->SoapPropertyDescriptor.push_back(
				DescriptorsUtil::GetSoapPropertyDescriptor( *propIt ) );
	}

	for( deviceIt = program.Devices.begin(); deviceIt != program.Devices.end(); deviceIt++ )
	{
		soapProgram->Devices->SoapDeviceDescriptor.push_back(
				DescriptorsUtil::GetSoapDeviceDescriptor( *deviceIt ) );
	}

	return soapProgram;
}

ns1__SoapDeviceDescriptor* DescriptorsUtil::GetSoapDeviceDescriptor( DeviceDescriptor device )
{
	ns1__SoapDeviceDescriptor* soapDevice = new ns1__SoapDeviceDescriptor();

	soapDevice->Properties = new ns1__ArrayOfSoapPropertyDescriptor();
	soapDevice->Pinouts = new ns1__ArrayOfSoapPinoutDescriptor();
	soapDevice->DeviceConnectionPoints = new ns1__ArrayOfSoapConnectionPointDescriptor();

	list<PropertyDescriptor>::iterator propIt;
	list<ConnectionPointDescriptor>::iterator cpIt;
	list<PinoutDescriptor>::iterator pinIt;

	for( propIt = device.Properties.begin(); propIt != device.Properties.end(); propIt++ )
	{
		soapDevice->Properties->SoapPropertyDescriptor.push_back(
				DescriptorsUtil::GetSoapPropertyDescriptor( *propIt ) );
	}

	for( cpIt = device.ConnectionPoints.begin(); cpIt != device.ConnectionPoints.end(); cpIt++ )
	{
		soapDevice->DeviceConnectionPoints->SoapConnectionPointDescriptor.push_back(
				DescriptorsUtil::GetSoapConnectionPointDescriptor( *cpIt ) );
	}

	for( pinIt = device.Pinouts.begin(); pinIt != device.Pinouts.end(); pinIt++ )
	{
		soapDevice->Pinouts->SoapPinoutDescriptor.push_back(
				DescriptorsUtil::GetSoapPinoutDescriptor( *pinIt ) );
	}

	return soapDevice;
}

ns1__SoapDriverDescriptor* DescriptorsUtil::GetSoapDriverDescriptor( DriverDescriptor driver )
{
	ns1__SoapDriverDescriptor* soapDriver = new ns1__SoapDriverDescriptor();

	soapDriver->Properties = new ns1__ArrayOfSoapPropertyDescriptor();
	soapDriver->SupportedCommands = new ns1__ArrayOfSoapCommandDescriptor();
	soapDriver->SupportedEvents = new ns1__ArrayOfSoapEventDescriptor;

	list<PropertyDescriptor>::iterator propIt;
	list<CommandDescriptor>::iterator cmdIt;
	list<EventDescriptor>::iterator eventIt;

	for( propIt = driver.Properties.begin(); propIt != driver.Properties.end(); propIt++ )
	{
		soapDriver->Properties->SoapPropertyDescriptor.push_back(
						DescriptorsUtil::GetSoapPropertyDescriptor( *propIt ) );
	}

	for( cmdIt = driver.SupportedCommands.begin(); cmdIt != driver.SupportedCommands.end(); cmdIt++ )
	{
		soapDriver->SupportedCommands->SoapCommandDescriptor.push_back(
						DescriptorsUtil::GetSoapCommandDescriptor( *cmdIt ) );
	}

	for( eventIt = driver.SupportedEvents.begin(); eventIt != driver.SupportedEvents.end(); eventIt++ )
	{
		soapDriver->SupportedEvents->SoapEventDescriptor.push_back(
				 DescriptorsUtil::GetSoapEventDescriptor( *eventIt ) );
	}

	return soapDriver;
}

ns1__SoapPropertyDescriptor* DescriptorsUtil::GetSoapPropertyDescriptor(PropertyDescriptor property)
{
	ns1__SoapPropertyDescriptor* soapProperty = new ns1__SoapPropertyDescriptor();

	soapProperty->Name = new string( property.Name );
	soapProperty->DataType = new string( property.DataType );
	soapProperty->Value = new string( property.Value );

	return soapProperty;
}


ns1__SoapWireDescriptor* DescriptorsUtil::GetSoapWireDescriptor( WireDescriptor descriptor )
{
	ns1__SoapWireDescriptor* soapWire = new ns1__SoapWireDescriptor();

	soapWire->Properties = new ns1__ArrayOfSoapPropertyDescriptor();
	soapWire->WireConnectionPoints = new ns1__ArrayOfSoapWireConnectionPointDescr();

	list<PropertyDescriptor>::iterator propIt;
	list<WireConnectionPointDescriptor>::iterator connIt;

	for( propIt = descriptor.Properties.begin(); propIt != descriptor.Properties.end(); propIt++ )
	{
		soapWire->Properties->SoapPropertyDescriptor.push_back(
							DescriptorsUtil::GetSoapPropertyDescriptor( *propIt ) );
	}

	for( connIt = descriptor.WireConnectionPoints.begin(); connIt != descriptor.WireConnectionPoints.end(); connIt++ )
	{
		soapWire->WireConnectionPoints->SoapWireConnectionPointDescr.push_back(
							DescriptorsUtil::GetSoapWireConnectionPointDescriptor( *connIt ) );
	}

	return soapWire;
}

ns1__SoapWireConnectionPointDescr* DescriptorsUtil::GetSoapWireConnectionPointDescriptor( WireConnectionPointDescriptor descriptor )
{
	ns1__SoapWireConnectionPointDescr* soapWireConnPoint = new ns1__SoapWireConnectionPointDescr();

	soapWireConnPoint->ConnectionPointDescriptorId = new ns1__SoapPropertyDescriptor();
	soapWireConnPoint->ConnectionPointDescriptorId->DataType = new string("int");
	soapWireConnPoint->ConnectionPointDescriptorId->Name = new string("ConnectionPointId");

	char vaueStr[21];
	sprintf( vaueStr, "%d", descriptor.ConnectionPointDescriptorId );
	string valueString = vaueStr;

	soapWireConnPoint->ConnectionPointDescriptorId->Value = new string(valueString);

	soapWireConnPoint->DeviceDescriptorId = new ns1__SoapPropertyDescriptor();
	soapWireConnPoint->DeviceDescriptorId->DataType = new string("int");
	soapWireConnPoint->DeviceDescriptorId->Name = new string("DeviceId");

	char deviceStr[21];
	sprintf( deviceStr, "%d", descriptor.DeviceDescriptorId );
	string devValueString = deviceStr;

	soapWireConnPoint->DeviceDescriptorId->Value = new string(devValueString);

	return soapWireConnPoint;
}

ns1__SoapConnectionPointDescriptor* DescriptorsUtil::GetSoapConnectionPointDescriptor( ConnectionPointDescriptor descriptor )
{
	ns1__SoapConnectionPointDescriptor* soapcp = new ns1__SoapConnectionPointDescriptor();
	soapcp->Properties = new ns1__ArrayOfSoapPropertyDescriptor();

	list<PropertyDescriptor>::iterator propIt;
	for( propIt = descriptor.Properties.begin(); propIt != descriptor.Properties.end(); propIt++ )
	{
		soapcp->Properties->SoapPropertyDescriptor.push_back(
			DescriptorsUtil::GetSoapPropertyDescriptor( *propIt ) );
	}

	return soapcp;
}

ns1__SoapCommandDescriptor* DescriptorsUtil::GetSoapCommandDescriptor( CommandDescriptor descriptor )
{
	ns1__SoapCommandDescriptor* soapCmd = new ns1__SoapCommandDescriptor();
	soapCmd->Properties = new ns1__ArrayOfSoapPropertyDescriptor();
	soapCmd->CommandParameters = new ns1__ArrayOfSoapCommandParameterDescriptor();

	list<PropertyDescriptor>::iterator propIt;
	list<CommandParameterDescriptor>::iterator cmdParamIt;

	for( propIt = descriptor.Properties.begin(); propIt != descriptor.Properties.end(); propIt++ )
	{
		soapCmd->Properties->SoapPropertyDescriptor.push_back(
			DescriptorsUtil::GetSoapPropertyDescriptor( *propIt ) );
	}

	for(cmdParamIt = descriptor.CommandParameters.begin(); cmdParamIt != descriptor.CommandParameters.end(); cmdParamIt++)
	{
		soapCmd->CommandParameters->SoapCommandParameterDescriptor.push_back(
				DescriptorsUtil::GetSoapCommandParameterDescriptor( *cmdParamIt ) );
	}

	return soapCmd;
}

ns1__SoapEventDescriptor* DescriptorsUtil::GetSoapEventDescriptor( EventDescriptor descriptor )
{
	ns1__SoapEventDescriptor* soapEvent = new ns1__SoapEventDescriptor();
	soapEvent->Properties = new ns1__ArrayOfSoapPropertyDescriptor();
	soapEvent->EventParameters = new ns1__ArrayOfSoapEventParameterDescriptor();

	list<PropertyDescriptor>::iterator propIt;
	list<EventParameterDescriptor>::iterator eventParamIt;

	for( propIt = descriptor.Properties.begin(); propIt != descriptor.Properties.end(); propIt++ )
	{
		soapEvent->Properties->SoapPropertyDescriptor.push_back(
			DescriptorsUtil::GetSoapPropertyDescriptor( *propIt ) );
	}

	for(eventParamIt = descriptor.EventParameters.begin(); eventParamIt != descriptor.EventParameters.end(); eventParamIt++)
	{
		soapEvent->EventParameters->SoapEventParameterDescriptor.push_back(
				DescriptorsUtil::GetSoapEventParameterDescriptor( *eventParamIt ) );
	}

	return soapEvent;
}

ns1__SoapPinoutDescriptor* DescriptorsUtil::GetSoapPinoutDescriptor( PinoutDescriptor descriptor )
{
	ns1__SoapPinoutDescriptor* soapParam = new ns1__SoapPinoutDescriptor();
	soapParam->Properties = new ns1__ArrayOfSoapPropertyDescriptor();
	soapParam->SupportedCommands = new ns1__ArrayOfSoapCommandDescriptor();
	soapParam->SupportedEvents = new ns1__ArrayOfSoapEventDescriptor();

	list<PropertyDescriptor>::iterator propIt;
	list<CommandDescriptor>::iterator cmdIt;
	list<EventDescriptor>::iterator eventIt;

	for( propIt = descriptor.Properties.begin(); propIt != descriptor.Properties.end(); propIt++ )
	{
		soapParam->Properties->SoapPropertyDescriptor.push_back(
			DescriptorsUtil::GetSoapPropertyDescriptor( *propIt ) );
	}

	for( cmdIt = descriptor.SupportedCommands.begin(); cmdIt != descriptor.SupportedCommands.end(); cmdIt++ )
	{
		soapParam->SupportedCommands->SoapCommandDescriptor.push_back(
					DescriptorsUtil::GetSoapCommandDescriptor( *cmdIt ) );
	}

	for( eventIt = descriptor.SupportedEvents.begin(); eventIt != descriptor.SupportedEvents.end(); eventIt++ )
	{
		soapParam->SupportedEvents->SoapEventDescriptor.push_back(
					DescriptorsUtil::GetSoapEventDescriptor( *eventIt ) );
	}

	return soapParam;
}

ns1__SoapCommandParameterDescriptor* DescriptorsUtil::GetSoapCommandParameterDescriptor( CommandParameterDescriptor descriptor )
{
	ns1__SoapCommandParameterDescriptor* soapParam = new ns1__SoapCommandParameterDescriptor();
	soapParam->Properties = new ns1__ArrayOfSoapPropertyDescriptor();

	list<PropertyDescriptor>::iterator propIt;
	for( propIt = descriptor.Properties.begin(); propIt != descriptor.Properties.end(); propIt++ )
	{
		soapParam->Properties->SoapPropertyDescriptor.push_back(
			DescriptorsUtil::GetSoapPropertyDescriptor( *propIt ) );
	}

	return soapParam;
}

ns1__SoapEventParameterDescriptor* DescriptorsUtil::GetSoapEventParameterDescriptor( EventParameterDescriptor descriptor )
{
	ns1__SoapEventParameterDescriptor* soapParam = new ns1__SoapEventParameterDescriptor();
	soapParam->Properties = new ns1__ArrayOfSoapPropertyDescriptor();

	list<PropertyDescriptor>::iterator propIt;
	for( propIt = descriptor.Properties.begin(); propIt != descriptor.Properties.end(); propIt++ )
	{
		soapParam->Properties->SoapPropertyDescriptor.push_back(
			DescriptorsUtil::GetSoapPropertyDescriptor( *propIt ) );
	}

	return soapParam;
}


// ---------------------------------------------------------------------------------------------

ProgramDescriptor* DescriptorsUtil::GetProgramDescriptor(
		ns1__SoapProgramDescriptor* descriptor )
{
	ProgramDescriptor* programDescr = new ProgramDescriptor();

	vector<ns1__SoapPropertyDescriptor*> properties =
			descriptor->Properties->SoapPropertyDescriptor;
	vector<ns1__SoapDeviceDescriptor*> devices =
			descriptor->Devices->SoapDeviceDescriptor;
	vector<ns1__SoapWireDescriptor*> wires =
			descriptor->Wires->SoapWireDescriptor;
	ns1__SoapWireDescriptor* powerWire = descriptor->PowerWire;

	vector<ns1__SoapPropertyDescriptor*>::iterator itProp;
	vector<ns1__SoapDeviceDescriptor*>::iterator itDev;
	vector<ns1__SoapWireDescriptor*>::iterator itWire;

	for ( itProp = properties.begin(); itProp != properties.end(); itProp++ )
	{
		programDescr->Properties.push_back(
				DescriptorsUtil::GetPropertyDescriptor( *itProp ) );
	}

	for ( itDev = devices.begin(); itDev != devices.end(); itDev++ )
	{
		programDescr->Devices.push_back( DescriptorsUtil::GetDeviceDescriptor(
				*itDev ) );
	}

	for ( itWire = wires.begin(); itWire != wires.end(); itWire++ )
	{
		programDescr->Wires.push_back( DescriptorsUtil::GetWireDescriptor(
				*itWire ) );
	}

	programDescr->PowerWire = DescriptorsUtil::GetWireDescriptor( powerWire );

	return programDescr;
}

PropertyDescriptor DescriptorsUtil::GetPropertyDescriptor(
		ns1__SoapPropertyDescriptor* descriptor )
{
	PropertyDescriptor pd( *descriptor->Name, *descriptor->DataType,
			*descriptor->Value );
	return pd;
}

DeviceDescriptor DescriptorsUtil::GetDeviceDescriptor(
		ns1__SoapDeviceDescriptor* descriptor )
{
	DeviceDescriptor deviceDescr;

	vector<ns1__SoapPropertyDescriptor*> properties =
			descriptor->Properties->SoapPropertyDescriptor;
	vector<ns1__SoapPropertyDescriptor*>::iterator itProp;

	for ( itProp = properties.begin(); itProp != properties.end(); itProp++ )
	{
		deviceDescr.Properties.push_back(
				DescriptorsUtil::GetPropertyDescriptor( *itProp ) );
	}

	vector<ns1__SoapConnectionPointDescriptor*> connPoints =
			descriptor->DeviceConnectionPoints->SoapConnectionPointDescriptor;
	vector<ns1__SoapConnectionPointDescriptor*>::iterator itConnPoint;

	for ( itConnPoint = connPoints.begin(); itConnPoint != connPoints.end(); itConnPoint++ )
	{
		deviceDescr.ConnectionPoints.push_back(
				DescriptorsUtil::GetConnectionPointDescriptor( *itConnPoint ) );
	}

	vector<ns1__SoapPinoutDescriptor*> pinouts = descriptor->Pinouts->SoapPinoutDescriptor;
	vector<ns1__SoapPinoutDescriptor*>::iterator itPin;

	for( itPin = pinouts.begin(); itPin != pinouts.end(); itPin++ )
	{
		deviceDescr.Pinouts.push_back(
				DescriptorsUtil::GetPinoutDescriptor( * itPin ));
	}

	return deviceDescr;
}

PinoutDescriptor DescriptorsUtil::GetPinoutDescriptor( ns1__SoapPinoutDescriptor* descriptor )
{
	PinoutDescriptor pinout;

		vector<ns1__SoapPropertyDescriptor*> properties =
				descriptor->Properties->SoapPropertyDescriptor;
		vector<ns1__SoapPropertyDescriptor*>::iterator itProp;

		for ( itProp = properties.begin(); itProp != properties.end(); itProp++ )
			pinout.Properties.push_back( DescriptorsUtil::GetPropertyDescriptor(
					*itProp ) );

		vector<ns1__SoapCommandDescriptor*> commandsDescr =
				descriptor->SupportedCommands->SoapCommandDescriptor;
		vector<ns1__SoapCommandDescriptor*>::iterator itCmd;

		for ( itCmd = commandsDescr.begin(); itCmd != commandsDescr.end(); itCmd++ )
			pinout.SupportedCommands.push_back(
					DescriptorsUtil::GetCommandDescriptor( *itCmd ) );

		vector<ns1__SoapEventDescriptor*> eventsDescr =
				descriptor->SupportedEvents->SoapEventDescriptor;
		vector<ns1__SoapEventDescriptor*>::iterator itEvent;

		for ( itEvent = eventsDescr.begin(); itEvent != eventsDescr.end(); itEvent++ )
			pinout.SupportedEvents.push_back( DescriptorsUtil::GetEventDescriptor(
					*itEvent ) );

		return pinout;
}

ConnectionPointDescriptor DescriptorsUtil::GetConnectionPointDescriptor(
		ns1__SoapConnectionPointDescriptor* descriptor )
{
	ConnectionPointDescriptor cpDescriptor;

	vector<ns1__SoapPropertyDescriptor*> properties =
				descriptor->Properties->SoapPropertyDescriptor;
	vector<ns1__SoapPropertyDescriptor*>::iterator itProp;

	for ( itProp = properties.begin(); itProp != properties.end(); itProp++ )
	{
		cpDescriptor.Properties.push_back(
				DescriptorsUtil::GetPropertyDescriptor( *itProp ) );
	}

	return cpDescriptor;
}

WireDescriptor DescriptorsUtil::GetWireDescriptor(
		ns1__SoapWireDescriptor* descriptor )
{
	WireDescriptor wireDescr;

	vector<ns1__SoapPropertyDescriptor*> properties =
			descriptor->Properties->SoapPropertyDescriptor;
	vector<ns1__SoapPropertyDescriptor*>::iterator itProp;

	for ( itProp = properties.begin(); itProp != properties.end(); itProp++ )
		wireDescr.Properties.push_back( DescriptorsUtil::GetPropertyDescriptor(
				*itProp ) );

	vector<ns1__SoapWireConnectionPointDescr*> wireConnections =
			descriptor->WireConnectionPoints->SoapWireConnectionPointDescr;
	vector<ns1__SoapWireConnectionPointDescr*>::iterator itWireCp;

	for ( itWireCp = wireConnections.begin(); itWireCp != wireConnections.end(); itWireCp++ )
		wireDescr.WireConnectionPoints.push_back(
				DescriptorsUtil::GetWireCPDescriptor( *itWireCp ) );

	return wireDescr;
}

WireConnectionPointDescriptor DescriptorsUtil::GetWireCPDescriptor(
		ns1__SoapWireConnectionPointDescr* descriptor )
{
	WireConnectionPointDescriptor wireCP;

	wireCP.ConnectionPointDescriptorId = atoi(
			descriptor->ConnectionPointDescriptorId->Value->c_str() );
	wireCP.DeviceDescriptorId = atoi(
			descriptor->DeviceDescriptorId->Value->c_str() );

	return wireCP;
}

PinoutMappingDescriptor DescriptorsUtil::GetPinoutMappingDescriptor(
		ns1__SoapPinoutDescriptor* descriptor )
{
	PinoutMappingDescriptor pinout;

	vector<ns1__SoapPropertyDescriptor*> properties =
			descriptor->Properties->SoapPropertyDescriptor;
	vector<ns1__SoapPropertyDescriptor*>::iterator itProp;

	for ( itProp = properties.begin(); itProp != properties.end(); itProp++ )
		pinout.Properties.push_back( DescriptorsUtil::GetPropertyDescriptor(
				*itProp ) );

	vector<ns1__SoapCommandDescriptor*> commandsDescr =
			descriptor->SupportedCommands->SoapCommandDescriptor;
	vector<ns1__SoapCommandDescriptor*>::iterator itCmd;

	for ( itCmd = commandsDescr.begin(); itCmd != commandsDescr.end(); itCmd++ )
		pinout.CommandMappings.push_back(
				DescriptorsUtil::GetCommandDescriptor( *itCmd ) );

	vector<ns1__SoapEventDescriptor*> eventsDescr =
			descriptor->SupportedEvents->SoapEventDescriptor;
	vector<ns1__SoapEventDescriptor*>::iterator itEvent;

	for ( itEvent = eventsDescr.begin(); itEvent != eventsDescr.end(); itEvent++ )
		pinout.EventMappings.push_back( DescriptorsUtil::GetEventDescriptor(
				*itEvent ) );

	return pinout;
}

ProgramMappingDescriptor* DescriptorsUtil::GetProgramMappingDescriptor(
		ns1__SoapProgramMappingDescriptor* descriptor )
{
	ProgramMappingDescriptor* mapping = new ProgramMappingDescriptor();

	vector<ns1__SoapPinoutDescriptor*> pinOutsDescr =
			descriptor->PinoutMappings->SoapPinoutDescriptor;
	vector<ns1__SoapPinoutDescriptor*>::iterator i;

	for ( i = pinOutsDescr.begin(); i != pinOutsDescr.end(); i++ )
		mapping->PinoutMappings.push_back(
				DescriptorsUtil::GetPinoutMappingDescriptor( *i ) );

	return mapping;
}

CommandDescriptor DescriptorsUtil::GetCommandDescriptor(
		ns1__SoapCommandDescriptor* descriptor )
{
	CommandDescriptor command;

	vector<ns1__SoapPropertyDescriptor*> properties =
			descriptor->Properties->SoapPropertyDescriptor;
	vector<ns1__SoapPropertyDescriptor*>::iterator itProp;

	for ( itProp = properties.begin(); itProp != properties.end(); itProp++ )
		command.Properties.push_back( DescriptorsUtil::GetPropertyDescriptor(
				*itProp ) );

	if ( descriptor->CommandParameters != NULL )
	{
		vector<ns1__SoapCommandParameterDescriptor*> commandParams =
				descriptor->CommandParameters->SoapCommandParameterDescriptor;
		vector<ns1__SoapCommandParameterDescriptor*>::iterator itCmdParam;

		for ( itCmdParam = commandParams.begin(); itCmdParam != commandParams.end(); itCmdParam++ )
			command.CommandParameters.push_back( DescriptorsUtil::GetCommandParameterDescriptor( *itCmdParam ) );
	}
	return command;
}

EventDescriptor DescriptorsUtil::GetEventDescriptor(
		ns1__SoapEventDescriptor* descriptor )
{
	EventDescriptor event;

	vector<ns1__SoapPropertyDescriptor*> properties =
			descriptor->Properties->SoapPropertyDescriptor;
	vector<ns1__SoapPropertyDescriptor*>::iterator itProp;

	for ( itProp = properties.begin(); itProp != properties.end(); itProp++ )
		event.Properties.push_back( DescriptorsUtil::GetPropertyDescriptor(
				*itProp ) );

	if ( descriptor->EventParameters != NULL )
	{
		vector<ns1__SoapEventParameterDescriptor*> eventParams = descriptor->EventParameters->SoapEventParameterDescriptor;
		vector<ns1__SoapEventParameterDescriptor*>::iterator itEvent;

		for ( itEvent = eventParams.begin(); itEvent != eventParams.end(); itEvent++ )
			event.EventParameters.push_back( DescriptorsUtil::GetEventParameterDescriptor( *itEvent ) );
	}

	return event;
}

CommandParameterDescriptor DescriptorsUtil::GetCommandParameterDescriptor(
		ns1__SoapCommandParameterDescriptor* descriptor )
{
	CommandParameterDescriptor param;

	vector<ns1__SoapPropertyDescriptor*> properties =
			descriptor->Properties->SoapPropertyDescriptor;
	vector<ns1__SoapPropertyDescriptor*>::iterator itProp;

	for ( itProp = properties.begin(); itProp != properties.end(); itProp++ )
		param.Properties.push_back( DescriptorsUtil::GetPropertyDescriptor(
				*itProp ) );

	return param;
}

EventParameterDescriptor DescriptorsUtil::GetEventParameterDescriptor(
		ns1__SoapEventParameterDescriptor* descriptor )
{
	EventParameterDescriptor param;

	vector<ns1__SoapPropertyDescriptor*> properties =
			descriptor->Properties->SoapPropertyDescriptor;
	vector<ns1__SoapPropertyDescriptor*>::iterator itProp;

	for ( itProp = properties.begin(); itProp != properties.end(); itProp++ )
		param.Properties.push_back( DescriptorsUtil::GetPropertyDescriptor(
				*itProp ) );

	return param;
}
