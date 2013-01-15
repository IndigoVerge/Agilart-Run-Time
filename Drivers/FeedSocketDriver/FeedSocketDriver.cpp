/*
 * FeedSocketDriver.cpp
 *
 *  Created on: Jan 7, 2013
 *      Author: dev
 */

#include <iostream>

#include "FeedSocketDriver.h"
#include "Consts.h"
#include "Logging/EventLogger.h"
#include "Command.h"
#include "CommandParameter.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define SOCKETDriver_NAME "Socket Driver"
#define SOCKETDriver_TYPE "SOCKETDriver"
#define SOCKETDriver_VERSION "0.0.1"
#define SOCKETDriver_DESCR ""
#define SOCKETDriver_PUBL "CompletIT Ltd."

#define SOCKETDriver_IP "Socket Driver Ip"
#define SOCKETDriver_PORT "Socket Driver Port"
#define SECKETDriver_FEED "Feed Name"


using namespace std;

FeedSocketDriver::FeedSocketDriver()
{
	Command initCommand( INIT_CMD );
	initCommand.AddParameter( new CommandParameter( SECKETDriver_FEED, NULL, STRING_TYPE ) );
	initCommand.AddParameter( new CommandParameter( SOCKETDriver_IP, NULL, STRING_TYPE ) );
	initCommand.AddParameter( new CommandParameter( SOCKETDriver_PORT, NULL, INT_TYPE ) );

	this->commands->push_back( initCommand );
	this->supportedComands->push_back( initCommand );

	Command setValueCommand( SETVAL_CMD );
	setValueCommand.AddParameter( new CommandParameter( VALUE_KEY, NULL, STRING_TYPE ) );

	this->commands->push_back( setValueCommand );
	this->supportedComands->push_back( setValueCommand );

	Command stopCommand( STOP_CMD );
	this->commands->push_back( stopCommand );
	this->supportedComands->push_back( stopCommand );

}

FeedSocketDriver::~FeedSocketDriver() {
	// TODO Auto-generated destructor stub
}

const char* FeedSocketDriver::Type()
{
	return SOCKETDriver_VERSION;
}

const char* FeedSocketDriver::Version()
{
	return SOCKETDriver_TYPE;
}

const char* FeedSocketDriver::Name()
{
	return SOCKETDriver_NAME;
}

const char* FeedSocketDriver::Description()
{
	return SOCKETDriver_DESCR;
}

const char* FeedSocketDriver::Publisher()
{
	return SOCKETDriver_PUBL;
}

void FeedSocketDriver::ExecuteCommand( Command command )
{
	string logMessage = "FeedsSocket Driver - Execute Command "	+ command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	if ( !strcmp( command.Name().c_str(), INIT_CMD ) )
	{
		CommandParameter* ipParam = command.GetParameter( SOCKETDriver_IP );
		this->ip = *(string*)(ipParam->Value);

		CommandParameter* portParam = command.GetParameter( SOCKETDriver_PORT );
		this->port = *(int*)(portParam->Value);

		FeedsSocketClient::Instance()->Initialize(this->ip, this->port);

	    string initMessage = "{\"command\":\"Init\", \"userId\"=\"1\"}";
	    FeedsSocketClient::Instance()->SendMessage(initMessage);
	}
	else if( !strcmp( command.Name().c_str(), STOP_CMD ) )
	{
		FeedsSocketClient::Instance()->Close();
	}
	else if( !strcmp( command.Name().c_str(), SETVAL_CMD) )
	{
		CommandParameter* ipParam = command.GetParameter( VALUE_KEY );
		string msg = *(string*)(ipParam->Value);

		string sendmsg = "{\"command\":\"PostData\", \"userId\":\"1\", \"feedId\":\"1\", \"date\":\"Today\", \"value\":\"" + msg + "\"}";
		FeedsSocketClient::Instance()->SendMessage( sendmsg );
	}
}

Command* FeedSocketDriver::GetCommand( const char* name )
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

list<Command> FeedSocketDriver::Commands()
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

BaseDriver* FeedSocketDriver::Clone()
{
	BaseDriver* driver = new FeedSocketDriver();
	return driver;
}

void ReportError(const char *msg)
{
    perror(msg);
    // exit(0);
}

