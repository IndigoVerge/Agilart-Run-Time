/*
 * FeedsSocketClient.cpp
 *
 *  Created on: Jan 9, 2013
 *      Author: dev
 */

#include "FeedsSocketClient.h"
#include "Logging/EventLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string>
#include <arpa/inet.h>

using namespace std;

FeedsSocketClient* FeedsSocketClient::instance = 0;

FeedsSocketClient::FeedsSocketClient()
{
	this->openConnectionsCount = 0;
	this->isInitialized = false;
}

FeedsSocketClient::~FeedsSocketClient()
{
	if( this->isInitialized == true )
		this->Close();

	delete instance;
}

FeedsSocketClient* FeedsSocketClient::Instance()
{
	if ( instance == NULL )
		instance = new FeedsSocketClient();

	return instance;
}

void FeedsSocketClient::Initialize(string ip, int portNumber)
{
	this->openConnectionsCount++;

    cout<<"initializing.. \n";

	if( this->isInitialized == true )
		return;

    int portno, n;
    struct sockaddr_in serv_addr;

    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
    	OnError("ERROR opening socket");
    	return;
    }

   memset(&serv_addr, 0, sizeof(serv_addr));                // zero the struct
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // set destination IP number
   serv_addr.sin_port = htons(portNumber);                // set destination port number

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
       printf ( "Error details: %s\n" ,strerror(errno));
       OnError("ERROR connecting");
       return;
    }

    cout<<"initialized.. \n";
    this->isInitialized = true;
}

void FeedsSocketClient::SendMessage( string message )
{
	if(this->isInitialized == false)
	{
		EventLogger::Instance()->WriteError("Cannot write to the socket channel as the client is not initialized");
		return;
	}

	cout<<"Sending message:"<<message<<"; ";
	char buffer[256];
	bzero(buffer,256);
	sprintf(buffer, "%s", message.c_str());

	int n = write(sockfd,buffer,strlen(buffer));

	cout<<" chars send: "<<n<<endl;

	if(n == -1)
	{
		printf ( "Sending failed, Error details: %s\n" ,strerror(errno));
	}
}

void FeedsSocketClient::Close()
{
	this->openConnectionsCount--;

	if(this->openConnectionsCount == 0)
	{
		cout<<"closing.. \n";
		close( this->sockfd );
		this->isInitialized = false;
	}
}

void FeedsSocketClient::OnError(const char *msg)
{
    EventLogger::Instance()->WriteError("Error in Feeds Scoket Server: %s", msg);
    exit(0);
}
