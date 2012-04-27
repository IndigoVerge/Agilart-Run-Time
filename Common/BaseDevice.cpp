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

#include "BaseDevice.h"
#include "InConnectionPoint.h"
#include "OutConnectionPoint.h"
#include <iostream>

using namespace std;

BaseDevice::BaseDevice()
{
	this->id = 0;
	this->isCustomDevice = false;
	this->isInitialized = false;
	this->isUpdating = false;

	this->properties = new list<DeviceParameter*>();
	this->pinouts = new std::list<Pinout*>();

	this->description = "Not Available";
	this->groupName = "Other";
}

BaseDevice::~BaseDevice()
{
	if ( this->connectionPoints != NULL )
	{
		list<ConnectionPoint*>::iterator cIt;

		for ( cIt = this->connectionPoints->begin(); cIt
				!= this->connectionPoints->end(); cIt++ )
		{
			if ( ( *cIt ) != NULL )
				delete ( *cIt );
		}

		delete this->connectionPoints;
	}

	if ( this->pinouts != NULL )
	{
		list<Pinout*>::iterator pIt;

		for ( pIt = this->pinouts->begin(); pIt != this->pinouts->end(); pIt++ )
		{
			if ( ( *pIt ) != NULL )
				delete ( *pIt );
		}

		delete this->pinouts;
	}

	if( this->properties != NULL )
		delete this->properties;
}

int BaseDevice::Id()
{
	return this->id;
}

void BaseDevice::SetId( int value )
{
	this->id = value;
}

void BaseDevice::SetName( string newName )
{
	this->name = newName;
}

void BaseDevice::SetIsCustom( bool isCustom )
{
	this->isCustomDevice = isCustom;
}

bool BaseDevice::GetIsCustom()
{
	return this->isCustomDevice;
}

void BaseDevice::SetIsUpdating( bool isUpdating )
{
	this->isUpdating = isUpdating;
}

bool BaseDevice::GetIsUpdating()
{
	return this->isUpdating;
}

void BaseDevice::WidgetInitialized()
{
	EventLogger::Instance()->WriteVerbose( "  BaseDevice::WidgetInitialized - %d", this->id );

	this->isInitialized = true;
	this->status = DEVICE_OK;

	list<ConnectionPoint*>::iterator it;
	for( it = this->connectionPoints->begin(); it != this->connectionPoints->end(); it++ )
	{
		if( (*it)->IsInitialized() == false )
		{
			// The conn point is not connected to a wire and therefore is still not initialed >
			// we need to initialize it by setting it's value to it's default value
			(*it)->SetIsInitialized( true );
			(*it)->SetState( UPDATED );
		}
	}
}

const char* BaseDevice::Name()
{
	return this->name.c_str();
}

const char* BaseDevice::Description()
{
	return this->description.c_str();
}

const char* BaseDevice::GroupName()
{
	return this->groupName.c_str();
}

bool BaseDevice::IsInitialized()
{
	return this->isInitialized;
}

list<Pinout*>* BaseDevice::Pinouts()
{
	return this->pinouts;
}

list<ConnectionPoint*> BaseDevice::ConnectionPoints()
{
	return *( this->connectionPoints );
}

ConnectionPoint* BaseDevice::GetConnectionPoint( int id )
{
	list<ConnectionPoint*>::iterator it;
	for ( it = this->connectionPoints->begin(); it
			!= this->connectionPoints->end(); it++ )
	{
		if ( ( *it )->Id() == id )
			return *it;
	}

	return NULL;
}


bool BaseDevice::AreAllINsInitialized()
{
	if ( this->isInitialized == true )
		return true;

	list<ConnectionPoint*>::iterator i;

	for ( i = this->connectionPoints->begin(); i
			!= this->connectionPoints->end(); i++ )
	{
		if ( ( *i )->Direction() == DIRECTION_OUT )
			continue;

		if ( ( *i )->IsCalculated() == false )
			return false;
	}

	return true;
}

DeviceDescriptor BaseDevice::GetDescriptor()
{
	DeviceDescriptor deviceDecriptor;

	stringstream ssId;
	ssId << this->id;
	PropertyDescriptor idProperty = PropertyDescriptor("Id", "int", ssId.str());
	deviceDecriptor.Properties.push_back( idProperty );

	list<ConnectionPoint*>::iterator it;

	for( it = this->connectionPoints->begin(); it != this->connectionPoints->end(); it++ )
		deviceDecriptor.ConnectionPoints.push_back((*it)->GetDescriptor());

	list<Pinout*>::iterator pinIt;

	for( pinIt = this->pinouts->begin(); pinIt != this->pinouts->end(); pinIt++)
		deviceDecriptor.Pinouts.push_back((*pinIt)->GetDecriptor());

	return deviceDecriptor;
}

void BaseDevice::AddCustomProperty( DeviceParameter* property )
{
	if( this->properties != NULL )
		this->properties->push_back( property );
}

void BaseDevice::InitializeFromDescriptor( list<PropertyDescriptor> properties )
{
	// Override in derived device widgets if required.
}

DeviceParameter BaseDevice::GetParameter( string parameterName )
{
	return DeviceParameter();
}

void BaseDevice::SetParameter( string parameterName, void* value )
{
	// Override in derived device widgets if required.
}

void BaseDevice::ExecuteCommand( Command command )
{
	// Override in derived device widgets if required.
}

void BaseDevice::EventTriggered( list<EventParameter> parameters )
{
	// Override in derived device widgets if required.
}

void BaseDevice::ReleaseResources()
{
	// Override in derived device widgets if required.
}

string BaseDevice::ToJson()
{
	char deviceId[16];
	sprintf( deviceId, "%d", this->Id() );

    string jsonDescr = "{\"" + JsonParameters::Id + "\": \"" + (string)deviceId + "\", \""
        + JsonParameters::ConnectionPoints + "\": [";

    list<ConnectionPoint*>::iterator connPointIt;
    list<Pinout*>::iterator pinIt;

    int i = 0;
    int count = this->connectionPoints->size();

    for ( connPointIt = this->connectionPoints->begin(); connPointIt != this->connectionPoints->end(); connPointIt++ )
    {
        i++;
        if ( i != count )
            jsonDescr += (*connPointIt)->ToJson() + ", ";
        else
            jsonDescr += (*connPointIt)->ToJson();
    }

    jsonDescr += "], \"" + JsonParameters::Pinouts + "\": [";
    i = 0;
    count = this->pinouts->size();

    for ( pinIt = this->pinouts->begin(); pinIt != this->pinouts->end(); pinIt++ )
    {
        i++;
        if ( i != count )
            jsonDescr += (*pinIt)->ToJson() + ", ";
        else
            jsonDescr += (*pinIt)->ToJson();
    }

    jsonDescr += "], \"" + JsonParameters::DeviceType + "\": \"" + this->Type() + "\", \"" + JsonParameters::DisplayName +
        "\": \"" + this->Name() + "\", \"" + JsonParameters::Description + "\": \"" +  this->Description() + "\", \"" + JsonParameters::Name + "\":\"" + this->Name() + "\",\""
        + JsonParameters::DeviceProperties + "\":[" ;

    list<DeviceParameter*>::iterator propertiesIt;
    count = this->properties->size();
    i = 0;

    for( propertiesIt = this->properties->begin(); propertiesIt != this->properties->end(); propertiesIt++ )
    {
    	jsonDescr += (*propertiesIt)->ToJson();
        i++;
		if ( i != count )
			jsonDescr += ", ";
    }

    string isCustom = this->isCustomDevice == true ? "true" : "false";

    jsonDescr += "],\"" + JsonParameters::CustomDevice + "\":" + isCustom + "}";

    return jsonDescr;
}

void BaseDevice::InputPinValueChanged( void* connPointPtr )
{
	InConnectionPoint* sender = ( InConnectionPoint* )connPointPtr;

	if( this->GetIsUpdating() == true )
	{
		this->SetIsUpdating( false );
		sender->SetState( UPDATED );
		// EventLogger::Instance()->WriteVerbose(">> BaseDevice::InputPinValueChanged - %d - already in UPDATING state", this->id );
		return;
	}

	// EventLogger::Instance()->WriteVerbose(">> BaseDevice::InputPinValueChanged - %d", this->id );

	this->SetIsUpdating( true );
	sender->SetState( UPDATED );

	list<ConnectionPoint*>::iterator i;
	for ( i = this->connectionPoints->begin(); i != this->connectionPoints->end(); i++ )
	{
		if ( ( *i )->Direction() == DIRECTION_OUT || (*i) == sender )
			continue;

		( *i )->SetState( INVALIDATED );
	}

	for ( i = this->connectionPoints->begin(); i != this->connectionPoints->end(); i++ )
	{
		if ( ( *i )->Direction() == DIRECTION_OUT || ( *i )->GetState() == UPDATED )
			continue;

		( *i )->SetState( UPDATING );
		( *i )->Refresh();
		( *i )->SetState( UPDATED );
	}

	// EventLogger::Instance()->WriteVerbose(">> BaseDevice::InputPinValueChanged - %d - Done.", this->id );

	this->CalculateOutputs();
	this->SetIsUpdating( false );

	if(this->IsInitialized() == false)
		this->WidgetInitialized();
}

void BaseDevice::Refresh( void* connPointPtr )
{
	if( this->GetIsUpdating() == true )
	{
		this->SetIsUpdating( false );
		// EventLogger::Instance()->WriteVerbose(">> BaseDevice::Refresh - %d - already in UPDATING state", this->id );
		return;
	}

	this->SetIsUpdating( true );

	// EventLogger::Instance()->WriteVerbose(">> BaseDevice::Refresh - %d", this->id );

	list<ConnectionPoint*>::iterator i;
	for ( i = this->connectionPoints->begin(); i != this->connectionPoints->end(); i++ )
	{
		if ( ( *i )->Direction() == DIRECTION_OUT )
			continue;

		( *i )->SetState( INVALIDATED );
	}

	for ( i = this->connectionPoints->begin(); i != this->connectionPoints->end(); i++ )
	{
		if ( ( *i )->Direction() == DIRECTION_OUT || ( *i )->GetState() == UPDATED )
			continue;

		( *i )->SetState( UPDATING );
		( *i )->Refresh();
		( *i )->SetState( UPDATED );
	}

	// EventLogger::Instance()->WriteVerbose(">> BaseDevice::Refresh - %d - Done.", this->id );
	this->CalculateOutputs();
	this->SetIsUpdating( false );

	if(this->IsInitialized() == false)
		this->WidgetInitialized();
}

void BaseDevice::OnError( int errorId )
{
	ErrorItem* error = new ErrorItem( errorId );
	this->OnError( error );
}

void BaseDevice::OnError( ErrorItem* error )
{
	if( this->errorsHandler != NULL )
	{
		(this->errorsHandler->*ErrorHandler)( this, error );
	}
}

void BaseDevice::RegisterErrorHandler( IErrorHandler* handler )
{
	this->ErrorHandler = &IErrorHandler::HandleError;
	this->errorsHandler = handler;
}
