/*
 * FeedSocketDriver.h
 *
 *  Created on: Jan 7, 2013
 *      Author: dev
 */

#ifndef FEEDSOCKETDRIVER_H_
#define FEEDSOCKETDRIVER_H_

#include "BaseDriver.h"
#include "FeedsSocketClient.h"

class FeedSocketDriver : public BaseDriver
{
	public:
		FeedSocketDriver();
		virtual ~FeedSocketDriver();

		virtual const char* Type();
		virtual const char* Version();
		virtual const char* Name();
		virtual const char* Description();
		virtual const char* Publisher();

		virtual void ExecuteCommand( Command );
		virtual Command* GetCommand( const char* );
		virtual std::list<Command> Commands();
		virtual BaseDriver* Clone();

	private:
		string ip;
		int port;
		int sockfd;
};

#endif /* FEEDSOCKETDRIVER_H_ */
