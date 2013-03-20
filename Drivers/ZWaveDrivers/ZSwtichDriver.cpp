/*
 * ZSwtichDriver.cpp
 *
 *  Created on: Dec 28, 2012
 *      Author: dev
 */

#include "ZSwtichDriver.h"
#include "Logging/EventLogger.h"
#include <iostream>

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "Options.h"
#include "Manager.h"
#include "Driver.h"
#include "Node.h"
#include "Group.h"
#include "Notification.h"
#include "ValueStore.h"
#include "Value.h"
#include "ValueBool.h"
#include "Log.h"

using namespace OpenZWave;

static uint32 g_homeId = 0;
static bool   g_initFailed = false;

typedef struct
{
	uint32			m_homeId;
	uint8			m_nodeId;
	bool			m_polled;
	list<ValueID>	m_values;
}NodeInfo;

static list<NodeInfo*> g_nodes;
static pthread_mutex_t g_criticalSection;
static pthread_cond_t  initCond  = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t initMutex = PTHREAD_MUTEX_INITIALIZER;

#define ZWaveDriver_PORT "Path"
#define ZWaveDriver_NAME "ZWave Swtich Driver"
#define ZWaveDriver_TYPE "ZSwtichDriver"
#define ZWaveDriver_VERSION "0.0.1"
#define ZWaveDriver_DESCR ""
#define ZWaveDriver_PUBL "CompletIT Ltd."

using namespace std;

NodeInfo* GetNodeInfo (	Notification const* _notification );
void OnNotification (	Notification const* _notification,	void* _context);

ZSwtichDriver::ZSwtichDriver()
{
	// TODO Auto-generated constructor stub
	this->port = "/dev/ttyUSB0";
	this->isInitialized = false;
//	this->InitDriver();

	EventLogger::Instance()->WriteDebug("ZSwtichDriver created successfully");
}

ZSwtichDriver::~ZSwtichDriver() {
	// TODO Auto-generated destructor stub
	if(this->isInitialized == true)
		this->CleanUp();
}

const char* ZSwtichDriver::Type() {
	return ZWaveDriver_TYPE;
}

const char* ZSwtichDriver::Version() {
	return ZWaveDriver_VERSION;
}

const char* ZSwtichDriver::Name() {
	return ZWaveDriver_NAME;
}

const char* ZSwtichDriver::Description() {
	return ZWaveDriver_DESCR;
}

const char* ZSwtichDriver::Publisher() {
	return ZWaveDriver_PUBL;
}

void ZSwtichDriver::ExecuteCommand(Command command) {

	string logMessage = "ZSwtichDriver - Execute Command "	+ command.Name();
//	EventLogger::Instance()->WriteInformation( logMessage.c_str() );
	cout<<"Driver:: "<<logMessage<<endl;

	if ( !strcmp( command.Name().c_str(), INIT_CMD ) )
	{
		CommandParameter* portParam = command.GetParameter( ZWaveDriver_PORT );
		this->port = *(string*)portParam->Value;
		cout<<"Path to ZWave plug: "<<this->port<<endl;
		this->InitDriver();
		this->Initialize();
		cout<<"Init Completed\n";
	}
	else if ( !strcmp( command.Name().c_str(), SETVAL_CMD ) )
	{
		CommandParameter* valueParam = command.GetParameter( VALUE_KEY );
		int value = *(int*) ( valueParam->Value );

		EventLogger::Instance()->WriteVerbose("ZSwtichDriver - Set Value - port %s  value - %d ", this->port.c_str(), value);
		this->SetValue( value == 0 ? false : true );
	}
	else if ( !strcmp( command.Name().c_str(), STOP_CMD ) )
	{
		this->CleanUp();
		cout<<"CleanUp Completed\n";
	}

}

Command* ZSwtichDriver::GetCommand(const char* name) {
	return NULL;
}

std::list<Command> ZSwtichDriver::Commands() {
}

BaseDriver* ZSwtichDriver::Clone() {
	BaseDriver* driver = new ZSwtichDriver();
	return driver;
}

void ZSwtichDriver::InitDriver() {

	pthread_mutexattr_t mutexattr;

	pthread_mutexattr_init ( &mutexattr );
	pthread_mutexattr_settype( &mutexattr, PTHREAD_MUTEX_RECURSIVE );
	pthread_mutex_init( &g_criticalSection, &mutexattr );
	pthread_mutexattr_destroy( &mutexattr );

	pthread_mutex_lock( &initMutex );

	// Create the OpenZWave Manager.
	// The first argument is the path to the config files (where the manufacturer_specific.xml file is located
	// The second argument is the path for saved Z-Wave network state and the log file.  If you leave it NULL
	// the log file will appear in the program's working directory.
	Options::Create( "./config/", "", "" );
	Options::Get()->AddOptionBool("ValidateValueChanges", true);
	Options::Get()->Lock();

	Manager::Create();
	Manager::Get()->AddWatcher( OnNotification, NULL );

	this->isInitialized = true;
}

void ZSwtichDriver::SetValue(bool value)
{
	pthread_mutex_lock( &g_criticalSection );
	for( list<NodeInfo*>::iterator it = g_nodes.begin(); it != g_nodes.end(); ++it )
	{
		NodeInfo* nodeInfo = *it;

		// skip the controller (most likely node 1)
		if( nodeInfo->m_nodeId != 3) continue;

		for( list<ValueID>::iterator it2 = nodeInfo->m_values.begin(); it2 != nodeInfo->m_values.end(); ++it2 )
		{
			ValueID v = *it2;
			if( v.GetCommandClassId() == 0x25 )
			{
				Manager::Get()->SetValue(v, value);
				EventLogger::Instance()->WriteDebug("Setting ZSwitchDriver value to: %s", value == true ? "True" : "False" );
				break;
			}
		}
	}

	pthread_mutex_unlock( &g_criticalSection );
}

void ZSwtichDriver::Initialize()
{
	Manager::Get()->AddDriver( this->port );
	pthread_cond_wait( &initCond, &initMutex );

	if( !g_initFailed )
	{
		Manager::Get()->WriteConfig( g_homeId );
	}
}

void ZSwtichDriver::CleanUp()
{
	Manager::Get()->RemoveDriver( this->port );

	Manager::Get()->RemoveWatcher( OnNotification, NULL );
	Manager::Destroy();
	Options::Destroy();
	pthread_mutex_destroy( &g_criticalSection );
}



//-----------------------------------------------------------------------------
// <GetNodeInfo>
// Return the NodeInfo object associated with this notification
//-----------------------------------------------------------------------------
NodeInfo* GetNodeInfo (	Notification const* _notification )
{
	uint32 const homeId = _notification->GetHomeId();
	uint8 const nodeId = _notification->GetNodeId();
	for( list<NodeInfo*>::iterator it = g_nodes.begin(); it != g_nodes.end(); ++it )
	{
		NodeInfo* nodeInfo = *it;
		if( ( nodeInfo->m_homeId == homeId ) && ( nodeInfo->m_nodeId == nodeId ) )
		{
			return nodeInfo;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// <OnNotification>
// Callback that is triggered when a value, group or node changes
//-----------------------------------------------------------------------------
void OnNotification (	Notification const* _notification,	void* _context)
{
	// Must do this inside a critical section to avoid conflicts with the main thread
	pthread_mutex_lock( &g_criticalSection );

	switch( _notification->GetType() )
	{
		case Notification::Type_ValueAdded:
		{
			if( NodeInfo* nodeInfo = GetNodeInfo( _notification ) )
			{
				// Add the new value to our list
				nodeInfo->m_values.push_back( _notification->GetValueID() );
			}
			break;
		}

		case Notification::Type_ValueRemoved:
		{
			if( NodeInfo* nodeInfo = GetNodeInfo( _notification ) )
			{
				// Remove the value from out list
				for( list<ValueID>::iterator it = nodeInfo->m_values.begin(); it != nodeInfo->m_values.end(); ++it )
				{
					if( (*it) == _notification->GetValueID() )
					{
						nodeInfo->m_values.erase( it );
						break;
					}
				}
			}
			break;
		}

		case Notification::Type_ValueChanged:
		{
			// One of the node values has changed
			if( NodeInfo* nodeInfo = GetNodeInfo( _notification ) )
			{
				printf("Value changed - node Id: %d \n", nodeInfo->m_nodeId );
				if ( nodeInfo->m_nodeId == 3) {
					for( list<ValueID>::iterator it2 = nodeInfo->m_values.begin(); it2 != nodeInfo->m_values.end(); ++it2 )
					{
						ValueID v = *it2;
						if( v.GetCommandClassId() == 0x25 ) {
						bool x;
						Manager::Get()->GetValueAsBool(v, &x);
						printf("Value: %s\n", x == false ? "False" : "True");
						}
					}
				}
			}
			break;
		}

		case Notification::Type_Group:
		{
			break;
		}

		case Notification::Type_NodeAdded:
		{
			// Add the new node to our list
			NodeInfo* nodeInfo = new NodeInfo();
			nodeInfo->m_homeId = _notification->GetHomeId();
			nodeInfo->m_nodeId = _notification->GetNodeId();
			nodeInfo->m_polled = false;
			g_nodes.push_back( nodeInfo );
			break;
		}

		case Notification::Type_NodeRemoved:
		{
			// Remove the node from our list
			uint32 const homeId = _notification->GetHomeId();
			uint8 const nodeId = _notification->GetNodeId();
			for( list<NodeInfo*>::iterator it = g_nodes.begin(); it != g_nodes.end(); ++it )
			{
				NodeInfo* nodeInfo = *it;
				if( ( nodeInfo->m_homeId == homeId ) && ( nodeInfo->m_nodeId == nodeId ) )
				{
					g_nodes.erase( it );
					delete nodeInfo;
					break;
				}
			}
			break;
		}

		case Notification::Type_NodeEvent:
		{
			break;
		}

		case Notification::Type_PollingDisabled:
		{
			if( NodeInfo* nodeInfo = GetNodeInfo( _notification ) )
			{
				nodeInfo->m_polled = false;
			}
			break;
		}

		case Notification::Type_PollingEnabled:
		{
			if( NodeInfo* nodeInfo = GetNodeInfo( _notification ) )
			{
				nodeInfo->m_polled = true;
			}
			break;
		}

		case Notification::Type_DriverReady:
		{
			g_homeId = _notification->GetHomeId();
			break;
		}

		case Notification::Type_DriverFailed:
		{
			g_initFailed = true;
			pthread_cond_broadcast(&initCond);
			break;
		}

		case Notification::Type_AwakeNodesQueried:
		case Notification::Type_AllNodesQueried:
		{
			pthread_cond_broadcast(&initCond);
			break;
		}

		case Notification::Type_DriverReset:
		case Notification::Type_NodeNaming:
		case Notification::Type_NodeProtocolInfo:
		case Notification::Type_NodeQueriesComplete:
		default:
		{
			break;
		}
	}

	pthread_mutex_unlock( &g_criticalSection );
}

