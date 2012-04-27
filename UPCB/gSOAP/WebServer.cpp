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
#include<iostream>
#include "WebServer.h"
#include "../Common/ProgramManager.h"
#include "../Common/Drivers/DriversManager.h"
#include "Logging/EventLogger.h"
#include "Consts.h"
#include <vector>

using namespace std;
int HttpGetHandler( struct soap *soap );
int HttpPutHandler( struct soap *soap );
int HttpDeleteHandler( struct soap *soap );

void SplitString( vector<string> &result, string str, char delim );
string ProcessClientRequest( string, string );
void SaveBoardSettings( string jsonDescription );

WebServer* WebServer::instance = 0;

WebServer::WebServer()
{
	this->isListening = false;
	this->soap = new UPCBWebServiceSoapService();
}

WebServer::~WebServer()
{
	//this->Stop();
}

WebServer* WebServer::Instance()
{
	if ( instance == NULL )
		instance = new WebServer();

	return instance;
}

void WebServer::Start( int p, char* ip )
{
	this->port = p;
	this->ip = ip;

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

void* WebServerThreadRun( void* param )
{
	WebServer* server = (WebServer*) param;

	int m, s; // master and slave sockets

	soap_init( server->soap );
	m = soap_bind( server->soap, server->ip, server->port, 100 );

	int (*http_get)( struct soap* ) = &HttpGetHandler;
	int (*http_put)( struct soap* ) = &HttpPutHandler;
	int (*http_delete)( struct soap* ) = &HttpDeleteHandler;

	( server->soap )->fget = http_get;
	( server->soap )->fput = http_put;
	( server->soap )->fdel = http_delete;

	server->soap->accept_timeout = 2; // in seconds

	if ( m < 0 )
		soap_print_fault( server->soap, stderr );
	else
	{
		server->isListening = true;
//		std::cout << "Socket connection successful: master socket = " << m
//				<< endl;

		for ( int i = 1;; i++ )
		{
			if( server->isListening == false )
			{
				break;
			}

			s = soap_accept( server->soap );

			if ( s < 0 && s != -1 )
			{
				std::cout << "soap_accept failed with error = " << s << endl;
				soap_print_fault( server->soap, stderr );
				break;
			}

			if( s == -1 )
			{
				// Timeout occurred.
				continue;
			}

			// cout<<"Serving Request...\n";

			int faultCode = server->soap->serve();
			if ( faultCode != SOAP_OK && faultCode != 6 ) // process RPC request
					 soap_print_fault( server->soap, stderr ); // print error

			if( server->soap->buf != NULL )
			{
				string msg = server->soap->buf;
				// cout<<"Soap buffer: "<<msg<<endl<<endl;
			}

			// This error code(6) denotes that a http POST message has arrived and
			// it doesn't have the expected for gsoap message stucture
			if( faultCode == 6 )
				server->HttpPostHandler( server->soap );

			if( server->soap->buf != NULL )
				memset( &server->soap->buf, 0, sizeof( server->soap->buf ) );

			soap_destroy( server->soap ); // clean up class instances
			soap_end( server->soap ); // clean up everything and close socket
		}
	}

	soap_destroy( server->soap ); // clean up class instances
	soap_end( server->soap ); // clean up everything and close socket
	soap_done( server->soap ); // close master socket and detach context

	free( server->soap );

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

int WebServer::HttpPostHandler( struct soap *soap )
{
	string contentLegthKey = "Content-Length: ";
	string newLineKey =  "\r\n";

	string message = soap->buf;
	size_t pos = message.find( newLineKey );

	string firstLine = message.substr( 0, pos );

	int urlStartIndex = firstLine.find( " " );
	string method = firstLine.substr( 0, urlStartIndex );

	if ( method != "POST" && method != "GET" )
	{
		return 404;
	}

	size_t urlEndIndex = firstLine.find( " ", urlStartIndex + 1 );
	string requestUrl = firstLine.substr( urlStartIndex + 2, urlEndIndex
			- urlStartIndex - 2 );

	size_t contentLengthIndex = message.find( contentLegthKey );
	pos = message.find( newLineKey, contentLengthIndex );

	contentLengthIndex += contentLegthKey.size();
	size_t fieldSize = pos - contentLengthIndex;
	string contentLengthValue = message.substr( contentLengthIndex, fieldSize );

	int contentLength = atoi( contentLengthValue.c_str() );

	pos = message.rfind( newLineKey );
	pos += newLineKey.size();
	string lastLine = ""; // The content of a POST request

	if( method == "POST" )
		lastLine = message.substr( pos, contentLength );

	if(lastLine == "")
	{
		cout<<" Error - couldn't read the response or the response was messed up !\n";
		return 200;
	}

	string response = ProcessClientRequest( requestUrl, lastLine );
	int length = response.length();

	char buf[length];
	sprintf( buf, "%s", response.c_str() );

	soap_response( soap, SOAP_HTML );
	soap_send( soap, buf );
	soap_end_send( soap );

	return SOAP_OK; //200
}

string ProcessClientRequest( string requestCommandName, string requestContent )
{
	string response = requestCommandName;

	vector<string> urlParts;
	SplitString( urlParts, requestCommandName, '/');

	if( urlParts[0] == ListProjectsOperation && urlParts.size() == 1 )
	{
		ProgramManager::Instance()->AddProgram( requestContent );
		//response = "";
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

int UpdateStatus( struct soap *soap )
{
	char buf[256];

	string result = "";
	//char *s = strchr( soap->path, '?' );
	// char *key = strtok( s, "=" );
	char *value = strtok( NULL, ";" );

	char* programName;
	int deviceId;
	char* propertyName;

	programName = strtok( value, "." );
	deviceId = atoi( strtok( NULL, "." ) );
	propertyName = strtok( NULL, "." );

	string propertyValue = "";

	while ( value )
	{
		//TODO - get by id
		Program* program = ProgramManager::Instance()->GetRunningProgram( 5 ); // programName );
		if ( program == NULL )
		{
			soap_response( soap, SOAP_HTML );
			sprintf( buf, "%s", "Program not started." );
			soap_send( soap, buf );
			soap_end_send( soap );

			return SOAP_OK;
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

		programName = strtok( NULL, "." );
		if ( programName == NULL )
			break;
		deviceId = atoi( strtok( NULL, "." ) );
		propertyName = strtok( NULL, "." );
	}

	sprintf( buf, "%s", result.c_str() );

	soap_response( soap, SOAP_HTML );
	soap_send( soap, buf );
	soap_end_send( soap );
	return SOAP_OK;
}

int SetScreenDeviceStatus( struct soap *soap )
{
	char buf[256];

	char *s = strchr( soap->path, '?' );

	char *key = query_key( soap, &s );
	char *value = query_val( soap, &s );

	int programId = atoi( strtok( key, "." ) );
	int deviceId = atoi( strtok( NULL, "." ) );
	char* propertyName = strtok( NULL, "." );

	bool isFound = false;

	Program* program = ProgramManager::Instance()->GetRunningProgram( programId );
	if ( program == NULL )
	{
		soap_response( soap, SOAP_HTML );
		sprintf( buf, "%s", "Program not started." );
		soap_send( soap, buf );
		soap_end_send( soap );

		return SOAP_OK;
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

	soap_response( soap, SOAP_HTML );
	sprintf( buf, "%s", "" );

	if ( isFound == false )
		sprintf( buf, "%s", "Device not found in the program definition." );

	soap_send( soap, buf );
	soap_end_send( soap );

	return SOAP_OK;
}

int CopyFile( struct soap *soap, const char *name, const char *type )
{
	FILE *fd;
	size_t r;
	fd = fopen( name, "rb" ); /* open file to copy */
	if ( !fd )
		return 404; /* return HTTP not found */
	soap->http_content = type;
	if ( soap_response( soap, SOAP_FILE ) ) /* OK HTTP response header */
	{
		soap_end_send( soap );
		fclose( fd );
		return soap->error;
	}
	for ( ;; )
	{
		r = fread( soap->tmpbuf, 1, sizeof( soap->tmpbuf ), fd );
		if ( !r )
			break;
		if ( soap_send_raw( soap, soap->tmpbuf, r ) )
		{
			soap_end_send( soap );
			fclose( fd );
			return soap->error;
		}
	}
	fclose( fd );
	return soap_end_send( soap );
}

int GetLogItems( struct soap *soap )
{
	string jsonDescr = EventLogger::Instance()->ToJson();
	int length = jsonDescr.length();

	char buf[length];
	sprintf( buf, "%s", jsonDescr.c_str() );

	soap_response( soap, SOAP_HTML );
	soap_send( soap, buf );
	soap_end_send( soap );

	return SOAP_OK;
}

int GetPrograms( struct soap *soap, string autogenerated )
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

	int length = jsonDescr.length();

	char buf[length];
	sprintf( buf, "%s", jsonDescr.c_str() );

	soap_response( soap, SOAP_HTML );
	soap_send( soap, buf );
	soap_end_send( soap );

	return SOAP_OK;
}

int GetProgram( struct soap *soap, string id )
{
	int programId = atoi( id.c_str() );
	string program = ProgramManager::Instance()->GetProgram( programId );

	int size = sprintf( soap->tmpbuf, "%s", program.c_str() );

	soap->http_content = "application/x-javascript; charset=utf-8"; // "text/javascript";

	soap_response( soap, SOAP_FILE );
	soap_send_raw( soap, soap->tmpbuf, size );

	soap_end_send( soap );
	return SOAP_OK;
}

int GetMapping( struct soap *soap, string id )
{
	int programId = atoi( id.c_str() );
	string mapping = ProgramManager::Instance()->GetMapping( programId );

	int size = sprintf( soap->tmpbuf, "%s", mapping.c_str() );

	soap->http_content = "application/x-javascript; charset=utf-8"; // "text/javascript";

	soap_response( soap, SOAP_FILE );
	soap_send_raw( soap, soap->tmpbuf, size );

	soap_end_send( soap );
	return SOAP_OK;
}

string GetRequestProgramName( string request )
{
	size_t nameIndex = request.find("name") + 4;
	int nameFieldSize = request.length() - nameIndex - 1;

	size_t ampersandIndex = request.find( '&', nameIndex );
	if( ampersandIndex != string::npos )
		nameFieldSize = ampersandIndex - nameIndex - 1;

	string programName = request.substr( nameIndex + 1, nameFieldSize );

	char* name = (char*)(programName.c_str());
	soap_decode_string(name, strlen(name) + 1, name);

	return programName;
}

int GetDriversRequest( struct soap *soap )
{
	string jsonDescr = DriversManager::Instance()->GetDriversJsonDescription();
	int length = jsonDescr.length();

	char buf[length];
	sprintf( buf, "%s", jsonDescr.c_str() );

	soap_response( soap, SOAP_HTML );
	soap_send( soap, buf );
	soap_end_send( soap );
	return SOAP_OK;
}

int GetDevicesRequest( struct soap *soap )
{
	string jsonDescr = DeviceManager::Instance()->GetDevicesJsonDescription();
	int length = jsonDescr.length();

	char buf[length];
	sprintf( buf, "%s", jsonDescr.c_str() );

	soap_response( soap, SOAP_HTML );
	soap_send( soap, buf );
	soap_end_send( soap );
	return SOAP_OK;
}

int StopProcess( struct soap *soap, string id )
{
	int programId = atoi( id.c_str() );
	int status = ProgramManager::Instance()->StopProgram( programId );
	string response = "";

	if(status != 0) {
		response = "{\"" + JsonParameters::OperationStatus + "\":500, \"" +
				JsonParameters::ErrorMessage + "\":\"" + EventLogger::Instance()->GetErrorMessage(status) + "\"}";
	}
	else {
		response = "{\"" + JsonParameters::OperationStatus + "\":200}";
	}

	char buf[response.length()];
	sprintf( buf, "%s", response.c_str() );

	soap_response( soap, SOAP_HTML );
	soap_send( soap, buf );
	soap_end_send( soap );
	return SOAP_OK;
}

int DeleteProgram( struct soap *soap, string id )
{
	int programId = atoi( id.c_str() );
	ProgramManager::Instance()->DeleteProgram( programId );

	string response = "";
	char buf[response.length()];
	sprintf( buf, "%s", response.c_str() );

	soap_response( soap, SOAP_HTML );
	soap_send( soap, buf );
	soap_end_send( soap );
	return SOAP_OK;
}

int GetProcessStatus( struct soap *soap, string id )
{
	int programId = atoi( id.c_str() );
	string programName = GetRequestProgramName( (string)soap->path );
	Program* program = ProgramManager::Instance()->GetRunningProgram( programId );
	string jsonDescr;

	if( program == NULL ) {
		// the program is not started
		jsonDescr += "{\"Status\":\"NotAvailable\"}";
		char buf[jsonDescr.length()];
		sprintf( buf, "%s", jsonDescr.c_str() );

		soap_response( soap, SOAP_HTML );
		soap_send( soap, buf );
		soap_end_send( soap );
		return SOAP_OK;
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

	char buf[jsonDescr.length()];
	sprintf( buf, "%s", jsonDescr.c_str() );

	soap_response( soap, SOAP_HTML );
	soap_send( soap, buf );
	soap_end_send( soap );
	return SOAP_OK;
}

int GetDeviceSettings( struct soap *soap )
{
	string jsonDescr = Configuration::Instance()->GetSettingsJsonDescription();
	int length = jsonDescr.length();

	char buf[length];
	sprintf( buf, "%s", jsonDescr.c_str() );

	soap_response( soap, SOAP_HTML );
	soap_send( soap, buf );
	soap_end_send( soap );
	return SOAP_OK;
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

int HttpDeleteHandler( struct soap *soap )
{
	string request = soap->path;
	size_t parametersIndex = request.find('?');

	if( parametersIndex != string::npos )
		request = request.substr( 0, parametersIndex );

	vector<string> urlParts;
	SplitString( urlParts, request, '/');

	if( urlParts[0] == ListProcessesOperation && urlParts.size() == 2 )
	{
		// /Processes/<Id>
		return StopProcess( soap, urlParts[1] );
	}
	else if( urlParts[0] == ListProjectsOperation && urlParts.size() == 2 )
	{
		// /Projects/<Id>
		return DeleteProgram( soap, urlParts[1] );
	}

	cout<<"(HttpDeleteHandler)Request not recognized - "<<request<<endl;
	return 404;
}

int HttpPutHandler( struct soap *soap )
{
	string contentLegthKey = "Content-Length: ";
	string newLineKey =  "\r\n";

	string message = soap->buf;
	size_t pos = message.find( newLineKey );
	string firstLine = message.substr( 0, pos );

	int urlStartIndex = firstLine.find( " " );
	string method = firstLine.substr( 0, urlStartIndex );

	if ( method != "PUT" )
		return 404;

	size_t urlEndIndex = firstLine.find( " ", urlStartIndex + 1 );
	string requestUrl = firstLine.substr( urlStartIndex + 2, urlEndIndex
			- urlStartIndex - 2 );

	size_t contentLengthIndex = message.find( contentLegthKey );
	pos = message.find( newLineKey, contentLengthIndex );

	contentLengthIndex += contentLegthKey.size();
	size_t fieldSize = pos - contentLengthIndex;
	string contentLengthValue = message.substr( contentLengthIndex, fieldSize );

	int contentLength = atoi( contentLengthValue.c_str() );

	pos = message.rfind( newLineKey );
	pos += newLineKey.size();
	string lastLine = message.substr( pos, contentLength );

	if(lastLine == "")
	{
		cout<<" Error - couldn't read the response or the response was messed up !\n";
		return 200;
	}

	string response = ProcessClientRequest( requestUrl, lastLine );
	int length = response.length();

	char buf[length];
	sprintf( buf, "%s", response.c_str() );

	soap_response( soap, SOAP_HTML );
	soap_send( soap, buf );
	soap_end_send( soap );

	return SOAP_OK;
}

int HttpGetHandler( struct soap *soap )
{
	if ( !soap_tag_cmp( soap->path, "*.xml" ) || !soap_tag_cmp(
			soap->path, "*.xsd" ) || !soap_tag_cmp( soap->path, "*.wsdl" ) )
	{
		return CopyFile( soap, soap->path + 1, "text/xml" );
	}
	else if ( !soap_tag_cmp( soap->path, "*.html" ) )
	{
		return CopyFile( soap, soap->path + 1, "text/html" );
	}
	else if ( !soap_tag_cmp( soap->path, "*.js" ) )
	{
		return CopyFile( soap, soap->path + 1, "text/javascript" );
	}
	else if ( !soap_tag_cmp( soap->path, "*.jpg" ) )
	{
		return CopyFile( soap, soap->path + 1, "image/jpg" );
	}
	else if ( !soap_tag_cmp( soap->path, "*.gif" ) )
	{
		return CopyFile( soap, soap->path + 1, "image/gif" );
	}
	else if ( !strncmp( soap->path, "/GetStatus?", 11 ) )
	{
		return UpdateStatus( soap );
	}
	else if ( !strncmp( soap->path, "/SetStatus?", 11 ) )
	{
		return SetScreenDeviceStatus( soap );
	}
	else if( ! strncmp( soap->path, "/favicon.ico", 12 ) )
	{
		return 403;
	}

	string request = soap->path;
	string backup = soap->path;

	request = request.substr( 1 ); // the request starts with a slash (/)

	// requestUrl?paramName=paramValue
	size_t parametersIndex = request.find('?');

	if( parametersIndex != string::npos )
		request = request.substr( 0, parametersIndex );

	vector<string> urlParts;
	SplitString( urlParts, request, '/');

	if( urlParts[0] == ListLogItemsOperation )
	{
		return GetLogItems( soap );
	}
	else if( urlParts[0] == ListProjectsOperation )
	{
		if(urlParts.size() == 1 )
		{
			return GetPrograms(soap, "");
		}
		else if(urlParts.size() == 2)
		{
			// /Programs/<Id>
			return GetProgram( soap, urlParts[1] );
		}
		else if(urlParts.size() == 3 && urlParts[2] == ProjectMappingOperation )
		{
			return GetMapping( soap, urlParts[1] );
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
			return GetProcessStatus( soap, urlParts[1] );
		}
	}
	else if( urlParts[0] == ListDevicesOperation )
	{
		return GetDevicesRequest( soap );
	}
	else if( urlParts[0] == ListDriversOperation )
	{
		return GetDriversRequest( soap );
	}
	else if( urlParts[0] == ListDeviceSettingsOperation )
	{
		return GetDeviceSettings( soap );
	}

	cout <<"GET Request for "<< soap->path << " is not available!"<<endl;
	return 404; /* HTTP not found */
}
