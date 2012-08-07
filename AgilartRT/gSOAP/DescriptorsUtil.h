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

#ifndef DESCRIPTORSUTIL_H_
#define DESCRIPTORSUTIL_H_

#include "Services/soapStub.h"
#include "Services/soapH.h"
#include "UPCBDescriptors.h"
#include <iostream>

class DescriptorsUtil
{
	public:
		DescriptorsUtil();
		virtual ~DescriptorsUtil();

		static ns1__SoapProgramDescriptor* GetSoapProgramDescriptor( ProgramDescriptor );
		static ns1__SoapProgramDescriptor* GetSoapProgramStatusDescriptor( ProgramDescriptor );

		static ns1__SoapPropertyDescriptor* GetSoapPropertyDescriptor( PropertyDescriptor );
		static ns1__SoapDeviceDescriptor* GetSoapDeviceDescriptor( DeviceDescriptor );
		static ns1__SoapDriverDescriptor* GetSoapDriverDescriptor( DriverDescriptor );
		static ns1__SoapConnectionPointDescriptor* GetSoapConnectionPointDescriptor( ConnectionPointDescriptor );
		static ns1__SoapPinoutDescriptor* GetSoapPinoutDescriptor( PinoutDescriptor );
		static ns1__SoapCommandDescriptor* GetSoapCommandDescriptor( CommandDescriptor );
		static ns1__SoapEventDescriptor* GetSoapEventDescriptor( EventDescriptor );
		static ns1__SoapWireDescriptor* GetSoapWireDescriptor( WireDescriptor );
		static ns1__SoapWireConnectionPointDescr* GetSoapWireConnectionPointDescriptor( WireConnectionPointDescriptor );
		static ns1__SoapCommandParameterDescriptor* GetSoapCommandParameterDescriptor( CommandParameterDescriptor );
		static ns1__SoapEventParameterDescriptor* GetSoapEventParameterDescriptor( EventParameterDescriptor );

		static bool GetStringProperty( list<PropertyDescriptor>, std::string key, std::string *result );
		static bool GetIntProperty( list<PropertyDescriptor>, std::string key, int *result );

		static ProgramDescriptor* GetProgramDescriptor( ns1__SoapProgramDescriptor* );
		static PropertyDescriptor GetPropertyDescriptor( ns1__SoapPropertyDescriptor* );
		static DeviceDescriptor GetDeviceDescriptor(  ns1__SoapDeviceDescriptor* );
		static WireDescriptor GetWireDescriptor(  ns1__SoapWireDescriptor* );
		static WireConnectionPointDescriptor GetWireCPDescriptor(  ns1__SoapWireConnectionPointDescr* );
		static PinoutMappingDescriptor GetPinoutMappingDescriptor(  ns1__SoapPinoutDescriptor* );
		static ProgramMappingDescriptor* GetProgramMappingDescriptor(  ns1__SoapProgramMappingDescriptor* );
		static CommandDescriptor GetCommandDescriptor(  ns1__SoapCommandDescriptor* );
		static EventDescriptor GetEventDescriptor(  ns1__SoapEventDescriptor* );
		static CommandParameterDescriptor GetCommandParameterDescriptor(  ns1__SoapCommandParameterDescriptor* );
		static EventParameterDescriptor GetEventParameterDescriptor(  ns1__SoapEventParameterDescriptor* );
		static ConnectionPointDescriptor GetConnectionPointDescriptor( ns1__SoapConnectionPointDescriptor*);
		static PinoutDescriptor GetPinoutDescriptor( ns1__SoapPinoutDescriptor* );

};

#endif /* DESCRIPTORSUTIL_H_ */
