/*
 * FeedsSocketServer.h
 *
 *  Created on: Jan 9, 2013
 *      Author: dev
 */

#include <string>
#include <iostream>

using namespace std;

#ifndef FEEDSSOCKETCLIENT_H_
#define FEEDSSOCKETCLIENT_H_

class FeedsSocketClient
{
	private:
		int openConnectionsCount;
		bool isInitialized;
		int sockfd;

		static FeedsSocketClient * instance;
		void OnError(const char *msg);

	protected:
		FeedsSocketClient();

	public:
		static FeedsSocketClient* Instance();

		virtual ~FeedsSocketClient();

		void Initialize(string ip, int portNumber);
		void SendMessage( string message );
		void Close();

};

#endif /* FEEDSSOCKETCLIENT_H_ */
