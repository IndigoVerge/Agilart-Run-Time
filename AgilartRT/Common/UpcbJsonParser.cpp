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

//#define PROFILE

#include "UpcbJsonParser.h"
#include "JsonParameters.h"
#include "Mappings/CommandMapping.h"
#include "Devices/DeviceManager.h"

using namespace std;

UpcbJsonParser* UpcbJsonParser::instance = 0;

UpcbJsonParser::UpcbJsonParser()
{
	// TODO Auto-generated constructor stub

}

UpcbJsonParser::~UpcbJsonParser()
{
	// TODO Auto-generated destructor stub
}

UpcbJsonParser* UpcbJsonParser::Instance()
{
	if ( instance == NULL )
		instance = new UpcbJsonParser();

	return instance;
}

ProgramMapping* UpcbJsonParser::ParseProgramMapping( string json )
{
	int i;
	size_t endIndex;

	size_t startFrom = 0;
	size_t start;
	size_t end;

	size_t temp;
	size_t index;
	int id;

	string commandsArray;
	string deviceId;
	string driverType;
	string eventsAray;
	string pinoutId;

	// Get Program Id
	i = json.find( JsonParameters::ProgramId );
	start = i + JsonParameters::ProgramId.size() + 2;
	index = json.find( JsonParameters::ProgramName );
	startFrom = json.find( JsonParameters::Mapping );

	end = index - 2;
	id = atoi( json.substr( start, end - start ).c_str() );

	start = index + JsonParameters::ProgramName.size() + 3;
	end = startFrom - 3;

#ifndef PROFILE
	ProgramMapping* mapping = new ProgramMapping( id, json.substr( start, end - start ) );
#else
	ProgramMapping* mapping = NULL;
#endif

	while ( true )
	{
		index = json.find( JsonParameters::Commands, startFrom );
		if ( index == string::npos)
			break;

		startFrom = index + 1;

		temp = json.find( JsonParameters::DeviceId, startFrom );

		// commandsIndex + [CommandParamName]  + ":
		start = index + JsonParameters::Commands.size() + 2;
		end = temp - 2;
		commandsArray = json.substr( start, end - start );

		start = temp + JsonParameters::DeviceId.size() + 2;
		temp = json.find( JsonParameters::Driver, startFrom );
		end = temp - 2;
		deviceId = json.substr( start, end - start );

		start = temp + JsonParameters::DriverType.size() + 3; // ":"
		temp = json.find( JsonParameters::Events, startFrom );
		end = temp - 3;
		driverType = json.substr( start, end - start );
		start = temp + JsonParameters::Events.size() + 2; // ":


		temp = json.find( JsonParameters::PinoutId, startFrom );
		endIndex = json.find( "}", temp );
		end = temp - 2;
		eventsAray = json.substr( start, end - start );

		start = temp + JsonParameters::PinoutId.size() + 2;
		end = endIndex;
		pinoutId = json.substr( start, end - start );

#ifndef PROFILE
		PinoutMapping* pinoutMapping = new PinoutMapping(atoi( pinoutId.c_str() ), atoi( deviceId.c_str() ), driverType);
#else
		PinoutMapping* pinoutMapping = NULL;
#endif

		i = 0;

		// Parse the commands
		this->ParseUpcbCommands( commandsArray, pinoutMapping );
		//Parse the events
		this->ParseUpcbEvents( eventsAray, pinoutMapping );

#ifndef PROFILE
		mapping->PinoutMappings()->push_back( pinoutMapping );
#endif
	}

	return mapping;
}

Program* UpcbJsonParser::ParseProgram( string json )
{
	size_t idIndex;
	size_t programDescrIndex;
	size_t devicesIndex;
	size_t nameIndex;
	size_t powerwireIndex;
	size_t wiresIndex;
	size_t devTypeIndex;
	size_t devPropsIndex;

	size_t start, end;
	size_t startIndex = 0;
	size_t startFrom = 0;
	size_t bracketIndex = 0;
	size_t keyIndex;
	size_t valueIndex;
	size_t typeIndex;

	string id;
	string programDescription;
	string devices;
	string name;
	string powerWire;
	string wires;
	string deviceType;
	string deviceProperties;
	string cpId;
	string propertyKey;
	string propertyValue;
	string propertyType;

	idIndex = json.find( JsonParameters::Id );
	programDescrIndex = json.find( JsonParameters::ProgramDescription );
	devicesIndex = json.find( JsonParameters::Devices );
	nameIndex = json.find( JsonParameters::Name );
	powerwireIndex = json.find( JsonParameters::PowerWire );
	wiresIndex = json.find( JsonParameters::Wires );

	start = idIndex + JsonParameters::Id.size() + 2; //":
	end = programDescrIndex - 2;
	id = json.substr( start, end - start );

	start = programDescrIndex + JsonParameters::ProgramDescription.size() + 3; //":"
	end = devicesIndex - 3;
	programDescription = json.substr( start, end - start );

	start = devicesIndex + JsonParameters::Devices.size() + 3; //":[
	end = nameIndex - 3;
	devices = json.substr( start, end - start );

	start = nameIndex + JsonParameters::Name.size() + 3; //":"
	end = powerwireIndex - 3;
	name = json.substr( start, end - start );

	start = powerwireIndex + JsonParameters::PowerWire.size() + 3;
	end = wiresIndex - 3;
	powerWire = json.substr( start, end - start );

	start = wiresIndex + JsonParameters::Wires.size() + 3;
	end = json.size() - 2;
	wires = json.substr( start, end - start );

#ifndef PROFILE
	Program* program = new Program( atoi( id.c_str() ), name, programDescription );
#else
	Program* program = NULL;
#endif

	BaseDevice* device;
	ConnectionPoint* connPoint;

	// Resolve Devices:
	while ( true )
	{
		idIndex = devices.find( JsonParameters::Id, startIndex );
		if ( idIndex == string::npos)
			break;

		devTypeIndex = devices.find( JsonParameters::DeviceType, startIndex );
		devPropsIndex = devices.find( JsonParameters::DeviceProperties, startIndex );

		start = idIndex + JsonParameters::Id.size() + 2;
		end = devTypeIndex - 2;
		id = devices.substr( start, end - start );

		start = devTypeIndex + JsonParameters::DeviceType.size() + 3;
		end = devPropsIndex - 3;
		deviceType = devices.substr( start, end - start );
#ifndef PROFILE
		device = DeviceManager::Instance()->GetDevice( deviceType.c_str() );
		if( device == NULL )
		{
			// LOG !
			return NULL;
		}

		device->SetId( atoi( id.c_str() ) );
		program->AddDevice( device );
#endif
		start = devPropsIndex + JsonParameters::DeviceProperties.size() + 2;
		end =  devices.find( ']', devPropsIndex ) + 1;
		deviceProperties = devices.substr( start, end - start );

		startFrom = 0;
		startIndex = end + 1;

		while( true )
		{
			bracketIndex = deviceProperties.find( '}', startFrom );
			if( bracketIndex == string::npos )
				break;

			keyIndex = deviceProperties.find( JsonParameters::DevicePropertyKey, startFrom );
			valueIndex = deviceProperties.find( JsonParameters::DevicePropertyValue, startFrom );
			typeIndex = deviceProperties.find( JsonParameters::DevicePropertyType, startFrom );

			start =  keyIndex + JsonParameters::DevicePropertyKey.size() + 3; // ":"
			end = valueIndex - 3;
			propertyKey = deviceProperties.substr( start, end - start );

			start =  valueIndex + JsonParameters::DevicePropertyValue.size() + 3;
			end = typeIndex - 3;
			propertyValue = deviceProperties.substr( start, end - start );

			start =  typeIndex + JsonParameters::DevicePropertyType.size() + 3;
			end = bracketIndex - 1;
			propertyType = deviceProperties.substr( start, end - start );

			string * valueString = new string(propertyValue);

			DeviceParameter* prop = new DeviceParameter( propertyKey, propertyType, propertyValue );
#ifndef PROFILE
			device->AddCustomProperty( prop );
			device->SetParameter(propertyKey, (void *)valueString);
#endif

			startFrom = bracketIndex + 1;
		}
	}

	//Resolve the Power wire
	int cpArrayIndex;
	string connPoints;
	int cpIdIndex, deviceIdIndex;
	string cpElement;

	//Resolve Wires
	startIndex = 0;
	while ( true )
	{
		startFrom = 0;
		cpArrayIndex = wires.find( JsonParameters::ConnectionPoints, startIndex );
		if ( cpArrayIndex == string::npos )
			break;

		startIndex = cpArrayIndex + 1;
		start = cpArrayIndex + JsonParameters::ConnectionPoints.size() + 3;
		end = wires.find( ']', startIndex );

		connPoints = wires.substr( start, end - start );
#ifndef PROFILE
		Wire* wire = new Wire();
#endif
		while ( true )
		{
			bracketIndex = connPoints.find( '}', startFrom );
			if ( bracketIndex == string::npos )
				break;

			cpElement = connPoints.substr( startFrom, bracketIndex - startFrom );
			startFrom = bracketIndex + 1;

			cpIdIndex = cpElement.find( JsonParameters::ConnectionPointId );
			deviceIdIndex = cpElement.find( JsonParameters::DeviceId );

			start = cpIdIndex + JsonParameters::ConnectionPointId.size() + 2;
			end = deviceIdIndex - 2;
			cpId = cpElement.substr( start, end - start );

			start = deviceIdIndex + JsonParameters::DeviceId.size() + 2;
			end = cpElement.size();
			id = cpElement.substr( start, end - start );
#ifndef PROFILE
			device = program->GetDevice( atoi( id.c_str() ) );
			connPoint = device->GetConnectionPoint( atoi( cpId.c_str() ) );

			wire->Attach( connPoint );
#endif
		}
#ifndef PROFILE
		program->Wires()->push_back( wire );
#endif
	}

	return program;
}

BaseDevice* UpcbJsonParser::ParseCustomDevice( string json )
{
	size_t deviceIdIndex;
	size_t nameIndex;
	size_t baseTypeIndex;
	size_t propertiesIndex;
	size_t startFrom = 0;
	size_t offset = 0;
	size_t start;
	size_t end;
	size_t endIndex;
	size_t bracketIndex;

	size_t keyIndex;
	size_t valueIndex;
	size_t typeIndex;

	string deviceName;
	string deviceBaseType;
	string deviceProperties;
	string deviceId; // SHOULD BE UNIQUE

	string propertyKey;
	string propertyValue;
	string propertyType;

	deviceIdIndex = json.find( JsonParameters::Id, startFrom );
	if ( deviceIdIndex == string::npos)
		return NULL;

	nameIndex = json.find( JsonParameters::CustomDeviceName, startFrom );
	baseTypeIndex = json.find( JsonParameters::CustomDeviceBaseType, startFrom );
	propertiesIndex = json.find( JsonParameters::CustomDeviceAddOnProps, startFrom );

	endIndex = json.find( "]", propertiesIndex );

	start = deviceIdIndex + JsonParameters::Id.size() + 2;
	end = nameIndex - 2;
	deviceId = json.substr( start, end - start );

	start = nameIndex + JsonParameters::CustomDeviceName.size() + 3;
	end = baseTypeIndex - 3;
	deviceName = json.substr( start, end - start );

	start = baseTypeIndex + JsonParameters::CustomDeviceBaseType.size() + 3;
	end = propertiesIndex - 3;
	deviceBaseType = json.substr( start, end - start );

	start = propertiesIndex + JsonParameters::CustomDeviceAddOnProps.size() + 3;
	end = endIndex;
	deviceProperties = json.substr( start, end - start );

#ifndef PROFILE
	BaseDevice* customDevice = DeviceManager::Instance()->GetDevice( deviceBaseType.c_str() );
	if(customDevice == NULL)
	{
		EventLogger::Instance()->WriteVerbose( "There is no device with base type - %s!", deviceBaseType.c_str() );
		return NULL;
	}

	customDevice->SetId( atoi( deviceId.c_str() ) );
	customDevice->SetName( deviceName );
#else
	BaseDevice* customDevice = NULL;
#endif

	offset = 0;

	// get properties:
	while( true )
	{
		bracketIndex = deviceProperties.find( '}', offset );
		if( bracketIndex == string::npos )
			break;

		keyIndex = deviceProperties.find( JsonParameters::DevicePropertyKey, offset );
		valueIndex = deviceProperties.find( JsonParameters::DevicePropertyValue, offset );
		typeIndex = deviceProperties.find( JsonParameters::DevicePropertyType, offset );

		start =  keyIndex + JsonParameters::DevicePropertyKey.size() + 3; // ":"
		end = valueIndex - 3;
		propertyKey = deviceProperties.substr( start, end - start );

		start =  valueIndex + JsonParameters::DevicePropertyValue.size() + 3;
		end = typeIndex - 3;
		propertyValue = deviceProperties.substr( start, end - start );

		start =  typeIndex + JsonParameters::DevicePropertyType.size() + 3;
		end = bracketIndex - 1;
		propertyType = deviceProperties.substr( start, end - start );

		DeviceParameter* property = new DeviceParameter(propertyKey, propertyType, propertyValue);

#ifndef PROFILE
		customDevice->AddCustomProperty( property );
#endif
		offset = bracketIndex + 1;
	}

	return customDevice;
}

void UpcbJsonParser::ParseUpcbCommands( string commandsArray, PinoutMapping* pinoutMapping )
{
	size_t endIndex;
	size_t startFrom = 0;
	size_t parameterStartFrom = 0;
	size_t start;
	size_t end;

	size_t sourceIndex;
	size_t targetIndex;
	size_t paramsIndex;
	size_t valueIndex;
	size_t dataTypeIndex;

     string parameters;
     string source;
     string target;
     string dataType;
     string cmdSource;
     string cmdTarget;
     string cmdValue;

     while ( true )
     {
         paramsIndex = commandsArray.find( JsonParameters::Parameters, startFrom );
         if ( paramsIndex == string::npos )
             break;
         startFrom = paramsIndex + 1;

         sourceIndex = commandsArray.find( JsonParameters::Source, paramsIndex );
         targetIndex = commandsArray.find( JsonParameters::Target, sourceIndex );
         endIndex = commandsArray.find( "}", targetIndex );

         start = paramsIndex + JsonParameters::Parameters.size() + 3; // ":[ - 3 characters
         end = sourceIndex - 3; // - ]," -
         parameters = commandsArray.substr( start, end - start );

         start = sourceIndex + JsonParameters::Source.size() + 3; // ":"
         end = targetIndex - 3;
         source = commandsArray.substr( start, end - start );

         start = targetIndex + JsonParameters::Target.size() + 3;
         end = endIndex - 1;
         target = commandsArray.substr( start, end - start );

         CommandMapping* cmdMapping = new CommandMapping( source, target );
#ifndef PROFILE
         pinoutMapping->Commands()->push_back( cmdMapping );
#endif

         parameterStartFrom = 0;
         while ( true )
         {
             dataTypeIndex = parameters.find( JsonParameters::DataType, parameterStartFrom );

             if ( dataTypeIndex == string::npos )
                 break;

             parameterStartFrom = dataTypeIndex + 1;
             sourceIndex = parameters.find( JsonParameters::ParameterSource, dataTypeIndex );
             targetIndex = parameters.find( JsonParameters::ParameterTarget, dataTypeIndex );
             valueIndex = parameters.find( JsonParameters::Value, dataTypeIndex );
             endIndex = parameters.find( "}", valueIndex );

             start = dataTypeIndex + JsonParameters::DataType.size() + 3;
             end = sourceIndex - 3;
             dataType = parameters.substr( start, end - start );

             start = sourceIndex + JsonParameters::ParameterSource.size() + 3;
             end = targetIndex - 3;
             cmdSource = parameters.substr( start, end - start );

             start = targetIndex + JsonParameters::ParameterTarget.size() + 3;
             end = valueIndex - 3;
             cmdTarget = parameters.substr( start, end - start );

             start = valueIndex + JsonParameters::Value.size() + 3;
             end = endIndex - 1;
             cmdValue = parameters.substr( start, end - start );

             void* cmdValuePointer = NULL;
             if( dataType == STRING_TYPE)
             {
            	 cmdValuePointer = new string( cmdValue );
             }
             else if( dataType == INT_TYPE )
             {
            	 cmdValuePointer = new int( atoi( cmdValue.c_str() ) );
             }
             else if( dataType == FLOAT_TYPE )
             {
            	 float value = atof( cmdValue.c_str() );
            	 cmdValuePointer = new float( value );
             }

             CommandParameterMapping* paramMapping =
            		 new CommandParameterMapping( cmdSource, cmdTarget, cmdValuePointer, dataType );

             cmdMapping->Parameters()->push_back( paramMapping );
         }
     }
}

void UpcbJsonParser::ParseUpcbEvents( string eventsArray, PinoutMapping* pinoutMapping )
{
    size_t endIndex;
    size_t startFrom = 0;
    size_t start;
    size_t end;

    size_t sourceIndex;
    size_t targetIndex;
    size_t paramsIndex;
    size_t valueIndex;
    size_t dataTypeIndex;
    size_t parameterStartFrom;

    string parameters;
    string source;
    string target;
    string dataType;
    string eventSource;
    string eventTarget;
    string eventValue;

    while ( true )
    {
        paramsIndex = eventsArray.find( JsonParameters::Parameters, startFrom );
        if ( paramsIndex == string::npos )
            break;
        startFrom = paramsIndex + 1;

        sourceIndex = eventsArray.find( JsonParameters::Source, paramsIndex );
        targetIndex = eventsArray.find( JsonParameters::Target, sourceIndex );
        endIndex = eventsArray.find( "}", targetIndex );

        start = paramsIndex + JsonParameters::Parameters.size() + 3; // ":[ - 3 characters
        end = sourceIndex - 3; // - ]," -
        parameters = eventsArray.substr( start, end - start );

        start = sourceIndex + JsonParameters::Source.size() + 3; // ":"
        end = targetIndex - 3;
        source = eventsArray.substr( start, end - start );

        start = targetIndex + JsonParameters::Target.size() + 3;
        end = endIndex - 1;
        target = eventsArray.substr( start, end - start );

        EventMapping* eventMapping = new EventMapping( source, target );
#ifndef PROFILE
        pinoutMapping->Events()->push_back( eventMapping );
#endif

        parameterStartFrom = 0;
        while ( true )
        {
            dataTypeIndex = parameters.find( JsonParameters::DataType, parameterStartFrom );

            if ( dataTypeIndex == string::npos )
                break;

            parameterStartFrom = dataTypeIndex + 1;
            sourceIndex = parameters.find( JsonParameters::ParameterSource, dataTypeIndex );
            targetIndex = parameters.find( JsonParameters::ParameterTarget, dataTypeIndex );
            valueIndex = parameters.find( JsonParameters::Value, dataTypeIndex );
            endIndex = parameters.find( "}", valueIndex );

            start = dataTypeIndex + JsonParameters::DataType.size() + 3;
            end = sourceIndex - 3;
            dataType = parameters.substr( start, end - start );

            start = sourceIndex + JsonParameters::ParameterSource.size() + 3;
            end = targetIndex - 3;
            eventSource = parameters.substr( start, end - start );

            start = targetIndex + JsonParameters::ParameterTarget.size() + 3;
            end = valueIndex - 3;
            eventTarget = parameters.substr( start, end - start );

            start = valueIndex + JsonParameters::Value.size() + 3;
            end = endIndex - 1;
            eventValue = parameters.substr( start, end - start );

            void* eventValuePointer = NULL;
		    if( dataType == "string ")
		    {
			   string* stringValue  = new string();
			   stringValue->append( eventValue );
			   eventValuePointer = stringValue;
		    }
		    else if( dataType == "int" )
		    {
			   int value = atoi( eventValue.c_str() );
			   eventValuePointer = new int( value );
		    }

            EventParameterMapping* paramMapping =
            		new EventParameterMapping( eventSource, eventTarget, eventValuePointer, dataType );

            eventMapping->Parameters()->push_back( paramMapping );
        }
    }
}
