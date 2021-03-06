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
#include <iostream>
#include "WebServer.h"
#include "../Common/ProgramManager.h"
#include "../Common/Drivers/DriversManager.h"
#include "Logging/EventLogger.h"
#include "Consts.h"
#include <vector>

using namespace std;

int HttpGetHandler( string, string, struct mg_connection* );
int HttpPostHandler( string, string, struct mg_connection* );
int HttpDeleteHandler( string, string, struct mg_connection* );

void SplitString( vector<string> &result, string str, char delim );
string ProcessClientRequest( string, string );
void SaveBoardSettings( string jsonDescription );


WebServer* WebServer::instance = 0;

WebServer::WebServer()
{
	this->isListening = false;
}

WebServer::~WebServer()
{

}

WebServer* WebServer::Instance()
{
	if ( instance == NULL )
		instance = new WebServer();

	return instance;
}

void WebServer::Start( char* port, char* serverip )
{
	this->serverPort = port;
	this->port = atoi(port);
	this->ip = serverip;
	this->WebServerStartListening();
}

int WebServer::Port()
{
	return this->port;
}

string WebServer::Ip()
{
	return this->ip;
}

void WebServer::Stop()
{
	this->isListening = false;
	pthread_join( this->webServerDoWorkThread, NULL );
}

void* CallbackHandler(enum mg_event event, struct mg_connection *conn)
{
  const struct mg_request_info *request_info = mg_get_request_info(conn);
  // EventLogger::Instance()->WriteVerbose("Executing: %s %s\n", request_info->request_method, request_info->uri);

  if( event == MG_HTTP_ERROR ) {
	  int http_reply_status_code = (long) request_info->ev_data;
	  EventLogger::Instance()->WriteVerbose("Request error found. Status code: %l\n", http_reply_status_code);
  }
  else if (event == MG_NEW_REQUEST) {

	  if( (string)request_info->request_method == "GET") {

		  string url = (string)request_info->uri;
		  if( url == "" || url == "/" )
			  return NULL; // the request won't be handled - let mongoose find another way to handle it

		  string query = "";
		  if(request_info->query_string != NULL)
			  query = (string)request_info->query_string;

		  if(HttpGetHandler((string)request_info->uri, query, conn) == 404)
			  return NULL; // the request won't be handled - let mongoose find another way to handle it

	  } else if( (string)request_info->request_method == "POST" || (string)request_info->request_method == "PUT" ) {

		  int length = -1;
		  for(int i=0; i<request_info->num_headers; i++) {
		  	  if((string)(request_info->http_headers[i].name) == "Content-Length")
		  		  length = atoi(request_info->http_headers[i].value);
		  }

		  if(length != -1 ) {
			  char* post_data = new char[length+1];
			  mg_read(conn, post_data, length);

			  post_data[length] = '\0';
			  HttpPostHandler((string)request_info->uri, (string)post_data, conn);
		  }
	  } else if( (string)request_info->request_method == "DELETE") {
		  string query = "";
		  if(request_info->query_string != NULL)
			  query = (string)request_info->query_string;

		  HttpDeleteHandler((string)request_info->uri, query, conn);
	  }

	  // Required by mongoose
	  void* return_value = new string();
	  return return_value;
  } else {
	return NULL;
  }
}

void* WebServerThreadRun( void* param )
{
	WebServer* server = (WebServer*) param;

	const char *options[] = {"listening_ports", server->serverPort, NULL};

	server->ctx = mg_start(&CallbackHandler, NULL, options);
	server->isListening = true;
	cout<<"Server started\n";

	while(server->isListening == true) {
		;
	}

	mg_stop(server->ctx);
	cout<<"Server killed\n";

	pthread_exit( (void *) 0 );
}

void WebServer::WebServerStartListening()
{
	int returnStatus = pthread_create( &this->webServerDoWorkThread, NULL,
			WebServerThreadRun, this );

	if ( returnStatus != 0 )
	{
		cout << "Error creating Web Server main thread!";
		return;
	}
}

int HttpPostHandler( string requestUrl, string content, struct mg_connection *conn )
{
	string response = ProcessClientRequest( requestUrl, content );
	int length = response.length();

	mg_printf(conn,
			  "HTTP/1.1 200 OK\r\n"
			  "Content-Type: application/x-javascript; charset=utf-8\r\n"
			  "Content-Length: %d\r\n"        // Always set Content-Length
			  "\r\n"
			  "%s",
			  length, response.c_str());

	return 200;
}

string ProcessClientRequest( string requestCommandName, string requestContent )
{
	string response = requestCommandName;

	vector<string> urlParts;
	SplitString( urlParts, requestCommandName, '/');

	if( urlParts[0] == ListProjectsOperation && urlParts.size() == 1 )
	{
		ProgramManager::Instance()->AddProgram( requestContent );
	}
	else if( urlParts[0] == ListProjectsOperation && urlParts.size() == 2 )
	{
		size_t isAutoStartIndex = requestContent.find( JsonParameters::ProgramAutoStart );
		if( isAutoStartIndex == string::npos )
		{
			ProgramManager::Instance()->AddProgram( requestContent );
		}
		else
		{
			isAutoStartIndex += JsonParameters::ProgramAutoStart.size() + 2;

			string isAutoStart = requestContent.substr(isAutoStartIndex, requestContent.size() - isAutoStartIndex - 1);
			int programId = atoi(urlParts[1].c_str());
			bool autoStart = isAutoStart == "true" ? true : false;
			ProgramManager::Instance()->SaveProgramAutoStartSetting( programId, autoStart );
		}
	}
	else if( urlParts[0] == ListProjectsOperation && urlParts.size() == 3 && urlParts[2] == ProjectMappingOperation )
	{
		// <ServiceAddress>/Projects/<Id>/Mapping
		ProgramManager::Instance()->AddProgramMapping( requestContent );
	}
	else if ( urlParts[0] == ListProcessesOperation )
	{
		// Parse requestContent > get project Id
		size_t idIndex = requestContent.find( JsonParameters::ProjectId );
		idIndex += JsonParameters::ProjectId.size() + 2;

		string idString = requestContent.substr(idIndex, requestContent.size() - idIndex - 1);
		int id = atoi(idString.c_str());

		int status = ProgramManager::Instance()->StartProgram(id);
		if(status != 0) {
			response = "{\"" + JsonParameters::OperationStatus + "\":500, \"" +
					JsonParameters::ErrorMessage + "\":\"" + EventLogger::Instance()->GetErrorMessage(status) + "\"}";
		}
		else {
			response = "{\"" + JsonParameters::OperationStatus + "\":200}";
		}
	}
	else if ( urlParts[0] == ListDevicesOperation )
	{
		ProgramManager::Instance()->SaveCustomDevice( requestContent );
		response = "";
	}
	else if( urlParts[0] == ListDeviceSettingsOperation )
	{
		SaveBoardSettings( requestContent );
		response = "";
	}

	return response;
}

void SaveBoardSettings( string jsonDescription )
{
	map<string,string> settings;

	size_t startFrom = 0;
	size_t nameIndex;
	size_t typeIndex;
	size_t valueIndex;
	size_t start;
	size_t end;

	string name;
	string value;
	size_t i;

	while(true)
	{
		nameIndex = jsonDescription.find( JsonParameters::SettingName, startFrom );
		if( nameIndex == string::npos )
			break;

		typeIndex = jsonDescription.find( JsonParameters::SettingDataType, nameIndex );
		valueIndex = jsonDescription.find( JsonParameters::SettingValue, typeIndex );

		start = nameIndex + JsonParameters::SettingName.size() + 3; // ":"
 		end = typeIndex - 3;
		name = jsonDescription.substr( start, end - start );

		start = valueIndex + JsonParameters::SettingValue.size() + 3; // ":"
 		end = jsonDescription.find( "}", valueIndex ) - 1;
		value = jsonDescription.substr( start, end - start );

		startFrom = end;

		i = value.find("\\/", 0);
		while( i != string::npos )
		{
			value.replace( i, 2, "/");
			i = value.find( "\\/", i+3 );
		}

		settings[name] = value;
	}

	Configuration::Instance()->SaveSettings( settings );
}

string GetDevicePropery( string query )
{
	string result = "";

	int programId = atoi( strtok( (char*)query.c_str(), "." ) );
	int deviceId = atoi( strtok( NULL, "." ) );
	char* propertyName = strtok( NULL, "." );

	string propertyValue = "";

	Program* program = ProgramManager::Instance()->GetRunningProgram( programId );
	if ( program == NULL )
	{
		cout<< "Program with id: "<<programId<<" is not started.\n";
		EventLogger::Instance()->WriteVerbose( "Program with id: %d is not started.", programId );

		return "";
	}

	list<BaseDevice*>::iterator it;

	for ( it = program->Devices()->begin(); it != program->Devices()->end(); it++ )
	{
		if ( ( *it )->Id() == deviceId )
		{
			//we have found the device we want -> set the Property we need
			DeviceParameter value = ( *it )->GetParameter( propertyName );

			if( value.IsInitialized() == true )
			{
				propertyValue = Convert::ToString( value.Value(), value.AgilartType() );
			}
			else
			{
				propertyValue = "0";
			}

			if ( result != "" )
				result += "-";

			result += propertyValue;

			break;
		}
	}

	return result;
}

void SetScreenDeviceStatus( string query )
{
	char *key = strtok( (char*)query.c_str(), "=" );
	char *value = strtok( NULL, ";" );

	int programId = atoi( strtok( key, "." ) );
	int deviceId = atoi( strtok( NULL, "." ) );
	char* propertyName = strtok( NULL, "." );

	bool isFound = false;

	Program* program = ProgramManager::Instance()->GetRunningProgram( programId );
	if ( program == NULL )
	{
		cout<< "Program with id: "<<programId<<" is not started.\n";
		EventLogger::Instance()->WriteVerbose("Program with id: %d is not started.", programId );

		return;
	}

	list<BaseDevice*>::iterator it;

	for ( it = program->Devices()->begin(); it != program->Devices()->end(); it++ )
	{
		if ( ( *it )->Id() == deviceId )
		{
			isFound = true;
			//we have found the device we want -> set the Property we need
			void* statusValue = new string( value );
			( *it )->SetParameter( propertyName, statusValue );

			break;
		}
	}

	if ( isFound == false )
	{
		cout<< "Device with id: "<<deviceId<<" is not available in program with id: "<<programId<<endl;
		EventLogger::Instance()->WriteVerbose("Device with id: %d is not available in program with id: %d", deviceId, programId );
	}
}

string GetLogItems()
{
	string jsonDescr = EventLogger::Instance()->ToJson();
	return jsonDescr;
}

string GetPrograms( string autogenerated )
{
	list<string> programs = ProgramManager::Instance()->GetAllPrograms();
	list<string>::iterator it;

	string jsonDescr = "";

	if( autogenerated != "" )
	{
		jsonDescr = autogenerated + "(";
	}

	jsonDescr += "[";
	int i = 0;

	for( it = programs.begin(); it != programs.end(); it++ )
	{
		if( (*it) == "" )
			continue;

		i++;
		jsonDescr += (*it) + ", ";
	}

	if( i > 0) {
		jsonDescr = jsonDescr.substr(0, jsonDescr.length() - 2);
	}
	jsonDescr += "]";
	if( autogenerated != "" )
	{
		jsonDescr += ")";
	}

	return jsonDescr;
}

string GetProgram( string id )
{
	string program = ProgramManager::Instance()->GetProgram( atoi( id.c_str() ) );
	return program;
}

string GetMapping( string id )
{
	string mapping = ProgramManager::Instance()->GetMapping( atoi( id.c_str() ) );
	return mapping;
}

string GetRequestProgramName( string request )
{
	size_t nameIndex = request.find("name") + 4;
	int nameFieldSize = request.length() - nameIndex - 1;

	size_t ampersandIndex = request.find( '&', nameIndex );
	if( ampersandIndex != string::npos )
		nameFieldSize = ampersandIndex - nameIndex - 1;

	string programName = request.substr( nameIndex + 1, nameFieldSize );

	// char* name = (char*)(programName.c_str());
	// soap_decode_string(name, strlen(name) + 1, name);

	return programName;
}


string GetDriversRequest()
{
	string jsonDescr = DriversManager::Instance()->GetDriversJsonDescription();
	return jsonDescr;
}

string GetDevicesRequest()
{
	string jsonDescr = DeviceManager::Instance()->GetDevicesJsonDescription();
	return jsonDescr;
}


string StopProcess( string id )
{
	int status = ProgramManager::Instance()->StopProgram( atoi(id.c_str()) );
	string response;

	if(status != 0) {
		response = "{\"" + JsonParameters::OperationStatus + "\":500, \"" +
				JsonParameters::ErrorMessage + "\":\"" + EventLogger::Instance()->GetErrorMessage(status) + "\"}";
	}
	else {
		response = "{\"" + JsonParameters::OperationStatus + "\":200}";
	}

	return response;
}

string DeleteProgram( string id )
{
	ProgramManager::Instance()->DeleteProgram( atoi( id.c_str() ) );
	return "";
}

string GetProcessStatus( string query, string id )
{
	// string programName = GetRequestProgramName( query );
	Program* program = ProgramManager::Instance()->GetRunningProgram( atoi( id.c_str() ) );
	string jsonDescr;

	if( program == NULL ) {
		// the program is not started
		jsonDescr += "{\"Status\":\"NotAvailable\"}";
		return jsonDescr;
	}

	string deviceStatus;
	string propertyName = "Value";
	DeviceParameter value;
	char deviceId[16];
	string status = "not set";
	char buffer[32];

	string deviceIp = WebServer::Instance()->Ip();
	int devicePort = WebServer::Instance()->Port();
	sprintf( buffer, "%d", devicePort );

	string processResource = "http://" + deviceIp + ":" + (string)buffer + "/Processes/" + id;
	string projectResource = "http://" + deviceIp + ":" + (string)buffer + "/Projects/" + id;
	string operation = GetProgramStatusOperation;
	jsonDescr = "{\"ResourceId\":\"" + processResource + "\",\"ProjectResourceId\":\"" + projectResource + "\",\"Devices\":[";
	string deviceUrl;
	string propertyUrl;

	list<BaseDevice*>::iterator it;

	for( it = program->Devices()->begin(); it != program->Devices()->end(); it++ )
	{
		value = ( *it )->GetParameter( propertyName );

		if ( value.IsInitialized() == true )
		{
			status = value.StringValue();
		}
		else
		{
			continue;
		}

		sprintf( deviceId, "%d", (*it)->Id() );
		// sprintf( status, "%d", statusValue );

		deviceUrl = processResource + "/Devices/" + (string)deviceId;
		deviceStatus = "{\"ResourceId\":\"" + deviceUrl + "\",\"Properties\":[{\"ResourceId\":\"" + deviceUrl + "/Properties/" + propertyName + "\",\"devkey\":\"" +
				propertyName + "\",\"devtype\":\"int\",\"devvalue\":\"" +  status + "\"}]},";
		jsonDescr += deviceStatus;
	}

	jsonDescr = jsonDescr.substr( 0, jsonDescr.length() - 1 ); // remove the last comma
 	jsonDescr += "],\"Status\":\"Running\"}";

	return jsonDescr;
}

string GetDeviceSettings()
{
	string jsonDescr = Configuration::Instance()->GetSettingsJsonDescription();
	return jsonDescr;
}

void SplitString( vector<string> &result, string str, char delim )
{
	  string tmp;
	  string::iterator i;
	  result.clear();

	  for(i = str.begin(); i <= str.end(); ++i)
	  {
		  if((const char)*i != delim  && i != str.end())
		  {
			  tmp += *i;
		  }
		  else
		  {
			  if(tmp != "")
				  result.push_back(tmp);

			  tmp = "";
		  }
	  }
}


int HttpDeleteHandler( string url, string query, struct mg_connection *conn )
{
	string response = "";
	string response_type = "application/x-javascript; charset=utf-8";

	vector<string> urlParts;
	SplitString( urlParts, url, '/');

	if( urlParts[0] == ListProcessesOperation && urlParts.size() == 2 )
	{
		// /Processes/<Id>
		response = StopProcess( urlParts[1] );
	}
	else if( urlParts[0] == ListProjectsOperation && urlParts.size() == 2 )
	{
		// /Projects/<Id>
		response = DeleteProgram( urlParts[1] );
	}
	else
	{
		cout <<"DELETE Request for "<< url << " is not available!"<<endl;
	}

	int length = response.length();

	mg_printf(conn,
			  "HTTP/1.1 200 OK\r\n"
			  "Content-Type: %s\r\n"
			  "Content-Length: %d\r\n"        // Always set Content-Length
			  "\r\n"
			  "%s",
			  response_type.c_str(), length, response.c_str());

	return 200;
}

int HttpGetHandler( string url, string query, struct mg_connection *conn )
{
	string response = "";
	string response_type = "application/x-javascript; charset=utf-8";

	vector<string> urlParts;
	SplitString( urlParts, url, '/');

	if ( urlParts[0] == "SetStatus" )
	{
		SetScreenDeviceStatus( query );
	}
	else if( urlParts[0] == "favicon.ico" )
	{
		return 403;
	}
	else if ( urlParts[0] == "GetStatus" )
	{
		response = GetDevicePropery( query );
	}
	else if( urlParts[0] == ListLogItemsOperation )
	{
		response = GetLogItems();
	}
	else if( urlParts[0] == ListProjectsOperation )
	{
		if(urlParts.size() == 1 )
		{
			response = GetPrograms("");
		}
		else if(urlParts.size() == 2)
		{
			// /Programs/<Id>
			response = GetProgram( urlParts[1] );
		}
		else if(urlParts.size() == 3 && urlParts[2] == ProjectMappingOperation )
		{
			response = GetMapping( urlParts[1] );
		}
	}
	else if( urlParts[0] == ListProcessesOperation )
	{
		if(urlParts.size() == 1 )
		{
			return 404; //GetProcesses(soap);
		}
		else if(urlParts.size() == 2)
		{
			// /Processes/<Id>
			response = GetProcessStatus( query, urlParts[1] );
		}
	}
	else if( urlParts[0] == ListDevicesOperation )
	{
		response = GetDevicesRequest();
	}
	else if( urlParts[0] == ListDriversOperation )
	{
		response = GetDriversRequest();
	}
	else if( urlParts[0] == ListDeviceSettingsOperation )
	{
		response = GetDeviceSettings();
	}
	else if( urlParts[0] == "About" )
	{
		char buffer[512];
		sprintf(buffer, "Server IP: %s Port Number: %d", WebServer::Instance()->Ip().c_str(), WebServer::Instance()->Port());
		response = (string)buffer;
	}
	else if( urlParts[0] == "clientaccesspolicy.xml" )
	{
		return 404;
	}
	else
	{
		EventLogger::Instance()->WriteVerbose("GET Request for %s is not available!\n", urlParts[0].c_str() );
		return 404;
	}

	int length = response.length();

	if(length != 0)
	{
		mg_printf(conn,
				  "HTTP/1.1 200 OK\r\n"
				  "Content-Type: %s\r\n"
				  "Content-Length: %d\r\n"        // Always set Content-Length
				  "\r\n"
				  "%s",
				  response_type.c_str(), length, response.c_str());
	}

	return 200;
}
