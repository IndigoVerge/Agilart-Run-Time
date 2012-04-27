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

#ifndef COMMAND_H_
#define COMMAND_H_

#include <list>
#include <iostream>
#include <string>

#include "CommandParameter.h"
#include "UPCBDescriptors.h"

using namespace std;

class Command
{
	private:

		string name;
		list<CommandParameter*>* parameters;

	public:
		Command();
		Command( string );
		Command( string, std::list<CommandParameter*>* );
		Command( const Command& param )
		{
			this->name = param.name;
			this->parameters = new list<CommandParameter*>();

			if(param.parameters != NULL)
			{
				list<CommandParameter*>::iterator it;
				for( it = param.parameters->begin(); it != param.parameters->end(); it++ )
				{
					this->parameters->push_back((*it)->Clone());
				}
			}
		}

		virtual ~Command();

		string Name();
		list<CommandParameter*>* Parameters();
		void AddParameter( CommandParameter* );

		CommandParameter* GetParameter( string );
		CommandDescriptor GetDescriptor();
		Command* Clone();

		virtual string ToJson();
};

#endif /* COMMAND_H_ */
