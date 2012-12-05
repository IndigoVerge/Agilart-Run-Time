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

#ifndef COMMANDMAPPING_H_
#define COMMANDMAPPING_H_

#include <string>
#include <list>

#include "CommandParameterMapping.h"
#include "UPCBDescriptors.h"
#include "Logging/EventLogger.h"

using namespace std;

class CommandMapping
{
	private:
		string source;
		string target;

		list<CommandParameterMapping*>* parameters;

	public:
		CommandMapping()
		{
			this->source = "";
			this->target = "";
			this->parameters = new list<CommandParameterMapping*>();
		}

		CommandMapping( string source, string target )
		{
			this->source = source;
			this->target = target;
			this->parameters = new list<CommandParameterMapping*>();
		}

		CommandMapping( CommandDescriptor *mapping)
		{
			DescriptorsUtil::GetStringProperty( mapping->Properties,
								"Source", &this->source );
			DescriptorsUtil::GetStringProperty( mapping->Properties,
								"Target", &this->target );

			this->parameters = new list<CommandParameterMapping*>();

			list<CommandParameterDescriptor>::iterator i;
			for(i = mapping->CommandParameters.begin(); i != mapping->CommandParameters.end(); i++)
			{
				CommandParameterMapping* param = new CommandParameterMapping( &(*i) );
				this->parameters->push_back( param );
			}
		}

		CommandMapping( const CommandMapping& param )
		{
			this->source = param.source;
			this->target = param.target;
			this->parameters = new list<CommandParameterMapping*>();

			if( param.parameters != NULL )
			{
				list<CommandParameterMapping*>::iterator it;
				for( it = param.parameters->begin(); it != param.parameters->end(); it++ )
				{
					if( (*it) != NULL )
						this->parameters->push_back( *it );
				}
			}
		}

		~CommandMapping()
		{
			if( this->parameters == NULL )
				return;
//
//			list<CommandParameterMapping*>::iterator it;
//			for( it = this->parameters->begin(); it != this->parameters->end(); it++ )
//			{
//				if( (*it) != NULL )
//					delete *it;
//			}
//
//			delete this->parameters;
		}

		string Source()
		{
			return this->source;
		}

		string Target()
		{
			return this->target;
		}

		CommandParameterMapping* GetCommapndParameterMapping( string sourceName )
		{
			list<CommandParameterMapping*>::iterator it;
			CommandParameterMapping* result;

			for( it = this->parameters->begin(); it != this->parameters->end(); it++ )
			{
				CommandParameterMapping* t = *it;

				if(sourceName == (*it)->Source())
				{
					result = *it;
					return result;
				}
			}

			EventLogger::Instance()->WriteVerbose( "Parameter with name %s is not found in the mappings.", sourceName.c_str() );
			return NULL;
		}

		list<CommandParameterMapping*>* Parameters()
		{
			return this->parameters;
		}
};

#endif /* COMMANDMAPPING_H_ */
