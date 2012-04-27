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

#ifndef CONSTS_H_
#define CONSTS_H_

#define VALUE_KEY "Value"
#define DIRECTION_KEY "direction"
#define PORTNUMBER_KEY "portnumber"

#define INIT_CMD "Initialize"
#define SETVAL_CMD "SetValue"
#define GETVAL_CMD "Get"
#define CHVAL_CMD "ValueChanged"
#define STOP_CMD "Stop"

//Used by GPIKeysDriver;
#define GPIO_NUMBER_PARAM "GPIONumber"
#define GPIOKEY_CODE_PARAM "Code"
#define GPIOKEY_TYPE_PARAM "EventType"
#define GPIOKEY_VALUE_PARAM "Value"

#define CHVAL_EVENT "ValueChanged"

#define PROGRAMS_STORAGE_DIR "ProgramsFolder"
#define CONFIG_STORAGE_DIR "StorageFolder"
#define CONFIG_Devices_DIR "devices"
#define CONFIG_AUTOSTARTIDS "AutoStartProgramIds"
#define CONFIG_MAXLOGSIZE "MaxLogSize"

#define DEVICES_DESCR_FILE "DeviceDescriptions.txt"
#define DEVICES_ENTYPOINT_SMBNAME "CreateDevices"

// Http request operations
#define ListLogItemsOperation "Log"
#define ListProjectsOperation "Projects"
#define ListDevicesOperation "Devices"
#define ListProcessesOperation "Processes"
#define ProjectMappingOperation "Mapping"
#define ListDriversOperation "Drivers"
#define ListDeviceSettingsOperation "DeviceSettings"

#define GetDriversOperation "GetDrivers"
#define GetDevicesOperation "GetDevices"
#define AddMappingOperation "AddMapping"
#define AddProgramOperation "AddProgram"
#define StartProgramOperation "StartProgram"
#define AddCustomDeviceOperation "AddCustomDevice"
#define StartProgramOperation "StartProgram"

#define GetAllProgramsOperation "GetAllPrograms"
#define GetProgramStatusOperation "GetProgramStatus"
#define StopProgramOperation "StopProgram"

#define ProcessStateRunning "Running"
#define ProcessStateError "Error"
#define ProcessStateNotAvailable "NotAvailable"

// set device parameter command constants
#define SET_PARAM_CMD "SetDeviceParameterCmd"
#define SET_PARAM_NAME "SetDeviceParameterName"
#define SET_PARAM_VAL "SetDeviceParameterValue"

#define STRING_TYPE "string"
#define INT_TYPE "int"
#define BOOL_TYPE "bool"
#define FLOAT_TYPE "float"
#define DOUBLE_TYPE "double"
#define ANY_TYPE "any"

#endif
