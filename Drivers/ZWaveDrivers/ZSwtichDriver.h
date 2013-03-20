/*
 * ZSwtichDriver.h
 *
 *  Created on: Dec 28, 2012
 *      Author: dev
 */

#ifndef ZSWTICHDRIVER_H_
#define ZSWTICHDRIVER_H_

#include "BaseDriver.h"
#include "Consts.h"

class ZSwtichDriver : public BaseDriver
{
	public:
		ZSwtichDriver();
		virtual ~ZSwtichDriver();

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
		string port;
		bool isInitialized;

		void InitDriver();
		void CleanUp();

		void Initialize();
		void SetValue(bool);
};

#endif /* ZSWTICHDRIVER_H_ */
