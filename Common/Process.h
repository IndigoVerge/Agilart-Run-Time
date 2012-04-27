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

#ifndef PROCESS_H_
#define PROCESS_H_

#include "Program.h"
#include "LogEntry.h"
#include "BaseDevice.h"
#include "Enums.h"
#include "Consts.h"

#include <list>

using namespace std;

class Process : public IErrorHandler
{
private:
	int id;
	Program* program;
	list<string> executionLog;
	ProcessState state;
	IErrorHandler* errorsHandler;
	ErrorItem* errorDescription;

	void (IErrorHandler::*ErrorHandler)(void* sender, ErrorItem* error);

public:
	Process();
	Process(int);
	virtual ~Process();

	void Start( Program* );
	void Stop();

	int Id();
	Program* GetProgram();
	ProcessState GetState();

	string GetStatus( string serverBaseAddress );

	virtual void HandleError( void* sender, ErrorItem* error);
	virtual void RegisterErrorHandler( IErrorHandler* handler );
};

#endif /* PROCESS_H_ */
