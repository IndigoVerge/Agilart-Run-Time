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
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string.h>
#include <string>
#include <stdio.h>
#include <map>
#define DEFAULT_CONFIG_FILE_PATH  "/etc/upcb.conf"

class Configuration
{
	private:
		static Configuration* instance;
		const char *configFilePath;
//		char *runFolderPath;
//		char *lockFilePath;
//		char *uid;
		void(*OnError)( std::string);

		std::map<std::string, std::string> configParameters;

	protected:
		Configuration();

	public:
		virtual ~Configuration();

		static Configuration* Instance();
		static void Initialize( const char *, void(*OnError)( std::string ) );

		std::string GetSettingsJsonDescription();
		void SaveSettings( std::map<std::string, std::string> settings );

		void SetParam( const char*, const char* );
		void SaveToConfigFile();
		std::string GetStringParam(std::string key);
		int GetIntParam(std::string key);
};

#endif /* CONFIGURATION_H_ */
