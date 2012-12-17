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
#ifndef EVENTMAPPING_H_
#define EVENTMAPPING_H_

#include <string>
#include <list>
#include "EventParameterMapping.h"

using namespace std;

class EventMapping
{
	private:
		string source;
		string target;

		list<EventParameterMapping*>* parameters;

	public:

		EventMapping()
		{
			this->source = "";
			this->target = "";
			this->parameters = new list<EventParameterMapping*>();
		}

		EventMapping(string source, string target)
		{
			this->source = source;
			this->target = target;
			this->parameters = new list<EventParameterMapping*>();
		}

		EventMapping( const EventMapping& param )
		{
			this->source = param.source;
			this->target = param.target;
			this->parameters = new list<EventParameterMapping*>();

			if( param.parameters != NULL )
			{
				list<EventParameterMapping*>::iterator it;
				for( it = param.parameters->begin(); it != param.parameters->end(); it++ )
				{
					if( (*it) != NULL )
						this->parameters->push_back( *it );
				}
			}
		}

		~EventMapping()
		{
			if( this->parameters == NULL )
				return;

			list<EventParameterMapping*>::iterator it;
			for( it = this->parameters->begin(); it != this->parameters->end(); it++ )
			{
				if( (*it) != NULL )
					delete *it;
			}

			delete this->parameters;
		}

		string Source()
		{
			return this->source;
		}

		string Target()
		{
			return this->target;
		}

		EventParameterMapping* GetEventParameterMapping( string sourceName )
		{
			list<EventParameterMapping*>::iterator it;
			EventParameterMapping* result;

			for( it = this->parameters->begin(); it != this->parameters->end(); it++ )
			{
				if(sourceName == (*it)->Source())
				{
					result = *it;
					break;
				}
			}

			return result;
		}

		list<EventParameterMapping*>* Parameters()
		{
			return this->parameters;
		}
};

#endif /* EVENTMAPPING_H_ */
