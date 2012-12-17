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

#include "UltraHeatDriver.h"

#define DriverType "NxThrera"
#define DriverVersion "0.0.1"
#define DriverName "NxThrera"
#define DriverPublisher "Unknown"
#define DriverDescription "not set"

#define PWR_PARAM "Set Power"
#define SERIAL_PORT "/dev/ttySAC"
#define COMPORT_NUM "Serial Port"

#include "errno.h"

UltraHeatDriver::UltraHeatDriver()
{
	Command initCommand( INIT_CMD );
	initCommand.AddParameter( new CommandParameter( COMPORT_NUM, NULL, "int" ) );

	Command setValueCommand( SETVAL_CMD );
	setValueCommand.AddParameter( new CommandParameter( VALUE_KEY, NULL, "int" ) );
	setValueCommand.AddParameter( new CommandParameter( PWR_PARAM, NULL, "int" ) );

	this->commands->push_back( initCommand );
	this->commands->push_back( setValueCommand );

	this->supportedComands->push_back( initCommand );
	this->supportedComands->push_back( setValueCommand );

	Command stopCommand( STOP_CMD );
	this->supportedComands->push_back( stopCommand );

	this->comPortFD = -1;
	this->comPortNumber = 0;

	char temp[50];
	sprintf(temp, "%s%d", SERIAL_PORT, this->comPortNumber);
	this->comPortName = temp;

	this->shouldStopReadDataThread = false;
	this->shouldWaitForResponse = false;
	this->readResponseTries = 0;
	this->responseWaitTime = 0.05; // = 10ms in seconds
}

UltraHeatDriver::~UltraHeatDriver()
{
	// implemented in the base
}

const char* UltraHeatDriver::Type()
{
	return DriverType;
}

const char* UltraHeatDriver::Version()
{
	return DriverVersion;
}

const char* UltraHeatDriver::Name()
{
	return DriverName;
}

const char* UltraHeatDriver::Description()
{
	return DriverDescription;
}

const char* UltraHeatDriver::Publisher()
{
	return DriverPublisher;
}

void UltraHeatDriver::ExecuteCommand( Command command )
{
	string logMessage = "UltraHeatDriver - Execute Command " + command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	if ( !strcmp( command.Name().c_str(), INIT_CMD ) )
	{
		CommandParameter* comNumberParam = command.GetParameter( COMPORT_NUM );
		this->comPortNumber = *(int*) ( comNumberParam->Value );

		char temp[50];
		sprintf( temp, "%s%d", SERIAL_PORT, this->comPortNumber );
		this->comPortName = temp;

		this->comPortFD = this->Open_port();
		this->StartReadDataThread();
		EventLogger::Instance()->WriteVerbose( "UltraHeatDriver - Opening serial port - %s", this->comPortName.c_str() );
	}
	else if ( !strcmp( command.Name().c_str(), SETVAL_CMD ) )
	{
		string data = "Exeuting SETVAL_CMD\n";
		write(this->fdDebug, data.c_str(), data.size() );

		CommandParameter* valueParam = command.GetParameter( VALUE_KEY );
		int value = *(int*) ( valueParam->Value );
		string message;

		if( value == 1 )
		{
			message = this->CreateMessage( HEAT_ON_CMDNAME, "" );
			EventLogger::Instance()->WriteVerbose( "UltraHeatDriver - HEAT ONN - %s", message.c_str() );
		}
		else
		{
			message = this->CreateMessage( HEAT_OFF_CMDNAME, "" );
			EventLogger::Instance()->WriteVerbose( "UltraHeatDriver - HEAT OFF - %s", message.c_str() );
		}

		this->WriteData( message );
	}
	else if ( !strcmp( command.Name().c_str(), STOP_CMD ) )
	{

		string logMessage = "UltraHeatDriver - Stop succeeded.";

		if( this->comPortFD == -1)
		{
			logMessage = "UltraHeatDriver - the COM hasn't been opened!";
		}
		else
		{
			this->Close_port( this->comPortFD );
		}

		EventLogger::Instance()->WriteVerbose( logMessage.c_str() );
		this->StopReadDataThread();
	}
	else if( ! strcmp( command.Name().c_str(), SET_PARAM_CMD ) )
	{
		CommandParameter* cmdParamKey = command.GetParameter( SET_PARAM_NAME );
		CommandParameter* cmdParamValue = command.GetParameter( SET_PARAM_VAL );

		if( cmdParamKey == NULL || cmdParamValue == NULL )
		{
			EventLogger::Instance()->WriteVerbose("The %s command should contain two parameters - %s and %s.", SET_PARAM_CMD, SET_PARAM_NAME, SET_PARAM_VAL);
			// throw
			return;
		}

		string key = *(string*)cmdParamKey->Value;
		string value = *(string*)cmdParamValue->Value;

		string message = this->CreateMessage( key, value );
		this->WriteData( message );
		EventLogger::Instance()->WriteVerbose( "UltraHeatDriver - %s", message.c_str() );
	}
	else
	{
		EventLogger::Instance()->WriteVerbose( "The command '%s' is not supported by the UltraHeatDriver.", command.Name().c_str() );
	}
}

list<Command> UltraHeatDriver::Commands()
{
	list<Command> commandsSet;
	list<Command>::iterator it;

	for ( it = this->commands->begin(); it != this->commands->end(); it++ )
	{
		Command* c = ( *it ).Clone();
		commandsSet.push_back( *c );
	}

	return commandsSet;
}

BaseDriver* UltraHeatDriver::Clone()
{
	BaseDriver* clone = new UltraHeatDriver();
	return clone;
}

Command* UltraHeatDriver::GetCommand( const char* name )
{
	list<Command>::iterator it;

	for ( it = this->commands->begin(); it != this->commands->end(); it++ )
	{
		if ( !strcmp( ( *it ).Name().c_str(), name ) )
		{
			return ( *it ).Clone();
		}
	}

	return NULL;
}

int UltraHeatDriver::Open_port(void)
{
	string debugMessage;

	this->fdDebug = open ( "debugFile.txt", O_RDWR | O_CREAT | O_TRUNC );
	if( this->fdDebug == -1 )
	{
		perror( "open_port: Unable to open debug port." );
		return -1;
	}

	int fd;
	EventLogger::Instance()->WriteVerbose( "Opening the serial port %s", this->comPortName.c_str() );

	fd = open( this->comPortName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK );
	if ( fd == -1 )
	{
		debugMessage = "Open serial port on: " + this->comPortName + " didn't succeed.\n";
		write( this->fdDebug, debugMessage.c_str(), debugMessage.size() );

		EventLogger::Instance()->WriteVerbose( "Unable to open serial port %s", this->comPortName.c_str() );
		perror( "open_port: Unable to open serial port." );
		return fd;
	}

	fcntl( fd, F_SETFL, FNDELAY );

	struct termios options;
	// Get the current options for the port...
	if( tcgetattr(fd, &options) != 0 )
	{
		debugMessage = "error setting the options\n " + (string)strerror(errno) + "\n";
		write( this->fdDebug, debugMessage.c_str(), debugMessage.length() );
		fsync( this->fdDebug );
	}

	// Set the baud rates
	if( cfsetispeed( &options, B115200 ) != 0 )
	{
		debugMessage = "error setting the options\n " + (string)strerror(errno) + "\n";
		write( fdDebug, debugMessage.c_str(), debugMessage.length() );
		fsync( fdDebug );
	}

	if( cfsetospeed( &options, B115200 ) != 0 )
	{
		debugMessage = "error setting the options\n " + (string)strerror(errno) + "\n";
		write( fdDebug, debugMessage.c_str(), debugMessage.length() );
		fsync( fdDebug );
	}

	// Enable options !!! MAGIC - these options are the default which are set when
	// the communication goes through the PC
	options.c_cflag = 0x00001cb2;
	options.c_oflag = 0x00000000;
	options.c_iflag = 0x00001400;
	options.c_lflag = 0x00008a20;

	// Write the options to the debug file, just in case of unexpected behavior
	char debugBuffer[1024];
	int bytesCount;

	bytesCount = sprintf( debugBuffer, "options cflag=%08x\n", options.c_cflag);
	write( fdDebug, debugBuffer, bytesCount );

	bytesCount = sprintf( debugBuffer, "options oflag=%08x\n", options.c_oflag);
	write( fdDebug, debugBuffer, bytesCount );

	bytesCount = sprintf( debugBuffer, "options iflag=%08x\n", options.c_iflag);
	write( fdDebug, debugBuffer, bytesCount );

	bytesCount = sprintf( debugBuffer, "options lflag=%08x\n", options.c_lflag);
	write( fdDebug, debugBuffer, bytesCount );

	bytesCount = sprintf( debugBuffer, "options line=%02x\n", options.c_line);
	write( fdDebug, debugBuffer, bytesCount );

	fsync( fdDebug );

	// Set the new options for the port
	if( tcsetattr(fd, TCSANOW, &options) != 0 )
	{
		debugMessage = "error setting the options\n " + (string)strerror(errno) + "\n";
		write( fdDebug, debugMessage.c_str(), debugMessage.length() );
		fsync( fdDebug );
	}

	debugMessage = "Open port succeeded.\n";
	write( this->fdDebug, debugMessage.c_str(), debugMessage.size() );

	return fd;
}

int UltraHeatDriver::Close_port( int fd )
{
	EventLogger::Instance()->WriteVerbose( "Closing the serial port %s", this->comPortName.c_str() );
	string debugMessage = "Close port succeeded.\n";
	write( this->fdDebug, debugMessage.c_str(), debugMessage.size() );

	int status = close( fd );

	return status;
}

int UltraHeatDriver::WriteData( string data )
{
	if( this->comPortFD == -1 )
	{
	 	EventLogger::Instance()->WriteVerbose( "The serial port %s is not opened", this->comPortName.c_str()  );
	 	perror( "write: The serial port is not opened." );

		return -1;
	}

	pthread_mutex_lock(&this->commonResourcesMutex);

	this->shouldWaitForResponse = true;
	this->readResponseTries = 0;
	int n = write( this->comPortFD, data.c_str(), data.size() );

	string debugMessage = "Writing data to the serial port: ";
	write(this->fdDebug, debugMessage.c_str(), debugMessage.size() );

	write(this->fdDebug, data.c_str(), data.size() );
	write(this->fdDebug, "\n", 1 );

	pthread_mutex_unlock(&this->commonResourcesMutex);

	 if (n < 0)
	 {
		 string debugMessage =  "Unable to write data to the serial port - " + data + "\n";
		 write( this->fdDebug, debugMessage.c_str(), debugMessage.size() );

	 	 EventLogger::Instance()->WriteVerbose( "Unable to write data(%s) to the serial port.", data.c_str() );
	 	 perror( "write: Unable to write data to the serial port." );
	 }

	 return n;
}

string UltraHeatDriver::CreateMessage( string key, string value )
{
	string stringCommand = "";

	stringCommand += MSG_START_BYTE;
	stringCommand += CMD_ADDRESS_BYTE;

	char buffer[2];
	char resultBuffer[10];

	resultBuffer[0] = *MSG_START_BYTE;
	resultBuffer[1] = *CMD_ADDRESS_BYTE;

	int argsCount = 3;
	int i;
	int crcByte = 0;

	if( key == SET_PWR_CMDNAME )
	{
		stringCommand += SET_PWR_CMDVALUE;
		int intValue = atoi( value.c_str() );
		sprintf ( buffer, "%02X", intValue );
		string stringValue = buffer;

		stringCommand += stringValue;
		argsCount = 5;

		resultBuffer[2] = *SET_PWR_CMDVALUE;
		resultBuffer[3] = buffer[0];
		resultBuffer[4] = buffer[1];
	}
	else if( key == HEAT_ON_CMDNAME )
	{
		stringCommand += HEAT_ON_CMDVALUE;
		resultBuffer[2] = *HEAT_ON_CMDVALUE;
	}
	else if( key == HEAT_OFF_CMDNAME )
	{
		stringCommand += HEAT_OFF_CMDVALUE;
		resultBuffer[2] = *HEAT_OFF_CMDVALUE;
	}

	char temp[16];

	for( i=0; i < argsCount; i++ )
	{
		crcByte -= (int)resultBuffer[i];
	}

	sprintf( temp, "%016X", crcByte );

	resultBuffer[argsCount] = temp[14];
	resultBuffer[argsCount + 1] = temp[15];
	stringCommand += temp[14];
	stringCommand += temp[15];

	stringCommand += MSG_END_BYTE;
	stringCommand += "\r\n";

	return stringCommand;
}


void* ReadDataThreadRun(void* param)
{
	UltraHeatDriver* driver = (UltraHeatDriver*) param;
	bool shouldLoop;
	clock_t endwait;

	char buffer[1024];
	int bytesRead;
	string dataReceived;

	int i;

	while (true)
	{
		shouldLoop = false;

		pthread_mutex_lock( &driver->commonResourcesMutex );
		if( driver->shouldWaitForResponse == false )
		{
			pthread_mutex_unlock(&driver->commonResourcesMutex);
			continue;
		}

		dataReceived = "";
		while( driver->readResponseTries < READ_TRIES_COUNT )
		{
			// read
			bytesRead = read( driver->comPortFD, buffer, 1024 );
			if( bytesRead == -1 && errno == EAGAIN ) // read didn't succeed
			{
				// wait responseWaitTime ms
				endwait = clock() + driver->responseWaitTime * CLOCKS_PER_SEC;
				while (clock() < endwait) {
						//wait
				}

				driver->readResponseTries = driver->readResponseTries + 1;

			}
			else if( bytesRead > 0 )
			{
				while( bytesRead > 0 )
				{
					char msg[bytesRead];

					for(i=0; i<bytesRead; i++)
					{
						msg[i] = buffer[i];
						cout<<buffer[i];
					}

					string debugMessage = "Data Received from the serial port: ";
					write(driver->fdDebug, debugMessage.c_str(), debugMessage.size() );

					write(driver->fdDebug, buffer, bytesRead );
					write(driver->fdDebug, "\n", 1 );

					dataReceived += (string)msg;
					bytesRead = read( driver->comPortFD, buffer, 1024 );
				}

				break;
			}
			else if( bytesRead == -1 )
			{
				 // cout<<"Error while reading: "<<strerror(errno)<<endl;
				 string debugMessage =  "Unable to read data from the serial port - " + (string)strerror(errno) + "\n";
				 write( driver->fdDebug, debugMessage.c_str(), debugMessage.size() );

				break;
			}
			else
			{
				break;
			}
		}

		driver->readResponseTries = 0;
		driver->shouldWaitForResponse = false;

		if( driver->readResponseTries < READ_TRIES_COUNT )
		{
			//cout<<"Bytes Read: "<<bytesRead<<" Data: "<<dataReceived<<" Buffer: "<<buffer<<endl;
			// write(driver->fdDebug, dataReceived.c_str(), dataReceived.size() );
			// write(driver->fdDebug, "\n", 1 );
		}
		else
		{
			//cout<<"The UltraHeat device didn't resond in a timely manner.\n";
			 string debugMessage =  "The device didn't respond in a timely manner.\n";
			 write( driver->fdDebug, debugMessage.c_str(), debugMessage.size() );

		}
		pthread_mutex_unlock(&driver->commonResourcesMutex);
	}

	pthread_exit((void *) 0);
}

void UltraHeatDriver::StartReadDataThread()
{
	int returnStatus = pthread_mutex_init(&this->commonResourcesMutex, NULL);

	if (returnStatus != 0)
	{
		cout << "Error creating UltraHeat Driver Common Resources Mutex!";
		return;
	}

	returnStatus = pthread_create(&this->readDataThread, NULL, ReadDataThreadRun,
			this);

	if (returnStatus != 0)
	{
		cout << "Error creating UltraHeat Driver thread!";
		return;
	}
}

void UltraHeatDriver::StopReadDataThread()
{
	pthread_mutex_lock(&this->commonResourcesMutex);
	this->shouldStopReadDataThread = true;
	pthread_mutex_unlock(&this->commonResourcesMutex);
}












