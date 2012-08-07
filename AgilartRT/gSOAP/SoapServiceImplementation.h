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
#ifndef SOAPSERVICEIMPLEMENTATION_H_
#define SOAPSERVICEIMPLEMENTATION_H_

#include <iostream>
#include "../Common/Program.h"
#include "../Common/Mappings/MappingManager.h"
#include "../Common/ProgramManager.h"
#include "../Common/Devices/DeviceManager.h"
#include "../Common/Drivers/DriversManager.h"

#include "Services/soapUPCBWebServiceSoapService.h"
#include "Services/UPCBWebServiceSoap.nsmap"
#include "Services/soapH.h"

#include "DescriptorsUtil.h"
#include "UPCBDescriptors.h"
#include "BaseDevice.h"

using namespace std;

int UPCBWebServiceSoapService::AddProgram( _ns1__AddProgram* param, _ns1__AddProgramResponse* response )
{
	// -------- Construct the program -----------

	ProgramDescriptor* upcbDescriptor = DescriptorsUtil::GetProgramDescriptor( param->program );
	ProgramMappingDescriptor* upcbProgramMapping = DescriptorsUtil::GetProgramMappingDescriptor( param->mapping );

	//ProgramManager::Instance()->AddProgram( upcbDescriptor, upcbProgramMapping );

	return SOAP_OK;
}

int UPCBWebServiceSoapService::StartProgram( _ns1__StartProgram *param, _ns1__StartProgramResponse *response )
{
	//ProgramManager::Instance()->StartProgram( *(param->programName) );

	return SOAP_OK;
}

int UPCBWebServiceSoapService::StopProgram( _ns1__StopProgram *param, _ns1__StopProgramResponse *response )
{
	//ProgramManager::Instance()->StopProgram( *(param->programName) );

	return SOAP_OK;
}

int UPCBWebServiceSoapService::GetDevices( _ns1__GetDevices *param, _ns1__GetDevicesResponse *response )
{
	list<DeviceDescriptor>* deviceDescriptors = DeviceManager::Instance()->GetDeviceDescriptors();
	list<DeviceDescriptor>::iterator it;

	response->GetDevicesResult = new ns1__ArrayOfSoapDeviceDescriptor();

	for( it = deviceDescriptors->begin(); it != deviceDescriptors->end(); it++ )
	{
		response->GetDevicesResult->SoapDeviceDescriptor.push_back(
				DescriptorsUtil::GetSoapDeviceDescriptor( *it ) );
	}

	return SOAP_OK;
}

int UPCBWebServiceSoapService::GetDrivers(_ns1__GetDrivers *param, _ns1__GetDriversResponse *response )
{
	list<DriverDescriptor>* driverDescriptors = DriversManager::Instance()->GetDriverDescriptors();
	list<DriverDescriptor>::iterator it;

	response->GetDriversResult = new ns1__ArrayOfSoapDriverDescriptor();

	for( it = driverDescriptors->begin(); it != driverDescriptors->end(); it++ )
	{
		response->GetDriversResult->SoapDriverDescriptor.push_back(
				DescriptorsUtil::GetSoapDriverDescriptor( *it ) );
	}

	return SOAP_OK;
}

int UPCBWebServiceSoapService::GetPrograms( _ns1__GetPrograms *param, _ns1__GetProgramsResponse *response)
{
//	list<ProgramDescriptor> upcbPrograms = ProgramManager::Instance()->GetAllPrograms();
//	list<ProgramDescriptor>::iterator it;
//
//	response->GetProgramsResult = new ns1__ArrayOfSoapProgramDescriptor();
//
//	for( it = upcbPrograms.begin(); it != upcbPrograms.end(); it++ )
//	{
//		response->GetProgramsResult->SoapProgramDescriptor.push_back(
//				DescriptorsUtil::GetSoapProgramDescriptor( *it ) );
//	}

	return SOAP_OK;
}

int UPCBWebServiceSoapService::GetProgramStatus(_ns1__GetProgramStatus *param, _ns1__GetProgramStatusResponse *response)
{
	if( param->programName == NULL )
		return SOAP_OK;

	//ProgramDescriptor upcbProgram = ProgramManager::Instance()->GetProgramStatus( *(param->programName) );
	//response->GetProgramStatusResult = DescriptorsUtil::GetSoapProgramDescriptor( upcbProgram );

	return SOAP_OK;
}

int UPCBWebServiceSoapService::GetProgram(_ns1__GetProgram *param, _ns1__GetProgramResponse *response)
{
	if( param->programName == NULL )
			return SOAP_OK;

	// ProgramDescriptor upcbProgram = ProgramManager::Instance()->GetProgram( *(param->programName) );
	// response->GetProgramResult = DescriptorsUtil::GetSoapProgramDescriptor( upcbProgram );

	return SOAP_OK;
}

int UPCBWebServiceSoapService::GetBoardInfo(_ns1__GetBoardInfo *param, _ns1__GetBoardInfoResponse *response)
{
	PropertyDescriptor modelProp("Model", "string", "UltraGUI");
	PropertyDescriptor versionProp("UPCBVersion", "string", "1.0.0");

	response->GetBoardInfoResult = new ns1__ArrayOfSoapPropertyDescriptor();

	response->GetBoardInfoResult->SoapPropertyDescriptor.push_back(
			DescriptorsUtil::GetSoapPropertyDescriptor( modelProp ) );
	response->GetBoardInfoResult->SoapPropertyDescriptor.push_back(
				DescriptorsUtil::GetSoapPropertyDescriptor( versionProp ) );

	return SOAP_OK;
}

int UPCBWebServiceSoapService::GetRunningProgramsStatus(_ns1__GetRunningProgramsStatus *param, _ns1__GetRunningProgramsStatusResponse *response)
{
	list<ProgramDescriptor> upcbPrograms = ProgramManager::Instance()->GetRunningProgramsStatus();
	list<ProgramDescriptor>::iterator it;

	response->GetRunningProgramsStatusResult = new ns1__ArrayOfSoapProgramDescriptor();

	for( it = upcbPrograms.begin(); it != upcbPrograms.end(); it++ )
	{
		response->GetRunningProgramsStatusResult->SoapProgramDescriptor.push_back(
				DescriptorsUtil::GetSoapProgramDescriptor( *it ) );
	}

	return SOAP_OK;
}


#endif /* SOAPSERVICEIMPLEMENTATION_H_ */
