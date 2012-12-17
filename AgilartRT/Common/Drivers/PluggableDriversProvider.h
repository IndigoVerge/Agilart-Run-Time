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

#ifndef PLUGGABLEDRIVERSPROVIDER_H_
#define PLUGGABLEDRIVERSPROVIDER_H_

#include "BaseDriversProvider.h"
#include "BaseDriver.h"

#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <map>
#include <list>
#include <stdlib.h>

class PluggableDriversProvider: public BaseDriversProvider
{
	private:
		char *plugindir;
		int GetDlError(const char *);

		typedef std::map<const char*, BaseDriver*> DriversMap;
		DriversMap drivers;

		void AddDriver( const char*, BaseDriver* );
		bool LoadSharedLibrary( string );

		bool InitializeUploadedDrivers();

	public:
		PluggableDriversProvider();
		virtual ~PluggableDriversProvider();

		virtual std::list<BaseDriver*> Drivers();
		virtual bool SupportsDriver( const char * );
		virtual BaseDriver* GetDriver( const char * );

		virtual bool Initialize();
};

#endif /* PLUGGABLEDRIVERSPROVIDER_H_ */
