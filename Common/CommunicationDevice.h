/*
 * CommunicationDevice.h
 *
 *  Created on: Sep 20, 2012
 *      Author: developer
 */

#ifndef COMMUNICATIONDEVICE_H_
#define COMMUNICATIONDEVICE_H_

#include <string>
#include <iostream>

using namespace std;

class CommunicationDevice {
public:
	CommunicationDevice();
	virtual ~CommunicationDevice();

	void SendData(string url, string data);
};

#endif /* COMMUNICATIONDEVICE_H_ */
