/*
 * SysfsInputDriver.cpp
 *
 *  Created on: Nov 27, 2012
 *      Author: developer
 */

#include "SysfsInputDriver.h"
#define SysfsInputDriver_NAME  "Sysfs Input Driver";
#define SysfsInputDriver_TYPE  "SysfsInput";
#define SysfsInputDriver_VERSION "0.0.1"
#define SysfsInputDriver_DESCR "Sysfs Input Description"
#define SysfsInputDriver_PUBL "CompletIT Ltd."

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64

SysfsInputDriver::SysfsInputDriver() : BaseDriver()
{
	Command initCommand( INIT_CMD );
	initCommand.AddParameter(
			new CommandParameter( GPIO_NUMBER_PARAM, NULL, "int" ) );

	this->commands->push_back( initCommand );
	this->supportedComands->push_back( initCommand );

	Event changeValueEvent( CHVAL_EVENT );
	changeValueEvent.AddParameter(EventParameter( GPIOKEY_CODE_PARAM, 0, "int" ));
	changeValueEvent.AddParameter(EventParameter( GPIOKEY_TYPE_PARAM, 0, "int" ));
	changeValueEvent.AddParameter(EventParameter( GPIOKEY_VALUE_PARAM, 0, "int" ));

	this->suportedEvents->push_back( changeValueEvent );

	Command stopCommand( STOP_CMD );
	this->supportedComands->push_back( stopCommand );
}

SysfsInputDriver::~SysfsInputDriver()
{
	// implemented in the BaseDriver
}

const char* SysfsInputDriver::Type()
{
	return SysfsInputDriver_TYPE;
}

const char* SysfsInputDriver::Version()
{
	return SysfsInputDriver_VERSION;
}

const char* SysfsInputDriver::Name()
{
	return SysfsInputDriver_NAME;
}

const char* SysfsInputDriver::Description()
{
	return SysfsInputDriver_DESCR;
}

const char* SysfsInputDriver::Publisher()
{
	return SysfsInputDriver_PUBL;
}

void* SysfsInputThreadRun( void* param )
{
	SysfsInputDriver* driver = (SysfsInputDriver*) param;

	int newStatus;

	while ( true )
	{
		pthread_mutex_lock( &driver->commonResourcesMutex );
		if(driver->shouldStop == true)
				break;
		pthread_mutex_unlock( &driver->commonResourcesMutex );

		newStatus = driver->GetGpioValue();

		if ( driver->GetStatus() != newStatus )
		{
			driver->SetStatus( newStatus );
			driver->SignalPortstatusChanged();
		}

		for ( int j = 0; j < 1000000; j++ )
			; // Delay
	}

	pthread_exit( (void *) 0 );
}

int SysfsInputDriver::GetStatus()
{
	return this->status;
}

void SysfsInputDriver::SetStatus( int value )
{
	this->status = value;
}

int SysfsInputDriver::GetPortNumber()
{
	return this->portNumber;
}

int SysfsInputDriver::GetGpioValue()
{
	int newStatus;
	int statusResult = this->gpio_get_value( this->portNumber, &newStatus );

	if ( statusResult != 0 )
	{
		cout << "Error in Sysfs GPIO get value\n";
		throw "Error in Sysfs GPIO get value";
	}

	return newStatus;
}

void SysfsInputDriver::SignalPortstatusChanged()
{
	list<EventParameter>* parameters = new list<EventParameter>();

	int portNumber = this->GetPortNumber();
	int curStaus = this->GetStatus();

	char numstr[21];
	sprintf( numstr, "%d", portNumber );
	string portNumberString = numstr;
	sprintf( numstr, "%d", curStaus );
	string curStausString = numstr;

	string logMessage = "SysfsInputDriver - SignalPortstatusChanged Port: " + portNumberString +
			" Status: " + curStausString;
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	parameters->push_back( EventParameter( GPIOKEY_CODE_PARAM, portNumber, "int" ) );
	parameters->push_back( EventParameter( GPIOKEY_VALUE_PARAM, curStaus, "int" ) );

	Event event( CHVAL_EVENT, parameters );

	this->StatusChangedEvent( this, event );
}

void SysfsInputDriver::StartPoolingForStatus()
{
	int returnStatus;

	returnStatus = pthread_mutex_init( &this->commonResourcesMutex, NULL );

	if ( returnStatus != 0 )
	{
		cout << "Error creating SysfsInputDriver driver Common Resources Mutex!";
		return;
	}

	returnStatus = pthread_create( &this->refreshStatusThread, NULL,
			SysfsInputThreadRun, this );

	if ( returnStatus != 0 )
	{
		cout << "Error creating SysfsInputDriver status update thread!";
		return;
	}
}

void SysfsInputDriver::StopPoolingForStatus()
{
	pthread_mutex_lock( &this->commonResourcesMutex );
	this->shouldStop = true;
	pthread_mutex_unlock( &this->commonResourcesMutex );

	this->ClosePort( this->portNumber );
	EventLogger::Instance()->WriteVerbose( "SysfsInputDriver - Stop thread succeeded. " );
}

void SysfsInputDriver::ExecuteCommand( Command command )
{
	string logMessage = "SysfsInputDriver - Execute Command " + command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	if ( !strcmp( command.Name().c_str(), INIT_CMD ) )
	{
		int outFlag = 0;

		CommandParameter* portnumberParam =	command.GetParameter( GPIO_NUMBER_PARAM );

		if ( this->portNumber != 0 )
			this->ClosePort( this->portNumber );

		this->portNumber = *(int*) ( portnumberParam->Value );
		this->shouldStop = false;
		this->OpenPort( this->portNumber, outFlag );
		// Trigger a 1-time event to notify the device of its current state.
		this->SignalPortstatusChanged();
		this->StartPoolingForStatus();
	}
	else if ( !strcmp( command.Name().c_str(), STOP_CMD ) )
	{
		this->StopPoolingForStatus();
	}
}

list<Command> SysfsInputDriver::Commands()
{
	list<Command> commandsSet;
	list<Command>::iterator it;

	for ( it = this->commands->begin(); it != this->commands->end(); it++ )
	{
		Command* c = ( *it ).Clone();
		commandsSet.push_back( *c );
	}

	return commandsSet;
}

BaseDriver* SysfsInputDriver::Clone()
{
	BaseDriver* clone = new SysfsInputDriver();
	return clone;
}

Command* SysfsInputDriver::GetCommand( const char* name )
{
	list<Command>::iterator it;

	for ( it = this->commands->begin(); it != this->commands->end(); it++ )
	{
		if ( !strcmp( ( *it ).Name().c_str(), name ) )
		{
			return ( *it ).Clone();
		}
	}

	return NULL;
}

int SysfsInputDriver::OpenPort( int portnumber, int isOutFlag )
{
	cout<<"OPEN PORT: "<<portnumber<<" isOut: "<<isOutFlag<<endl;

	int exportResult =  0; // this->gpio_export( portnumber );

	// if ( exportResult < 0 )
		// throw "Cannot export gpio";

	exportResult = this->gpio_set_dir( portnumber, isOutFlag );

	if ( exportResult < 0 )
		cout<<"gpio_set_dir did not work!\n";
		// throw "Cannot set gpio direction";

	int fd, len;
	char buf[MAX_BUF];

	len = snprintf( buf, sizeof( buf ), SYSFS_GPIO_DIR "/gpio%d/value",
			portnumber );

	fd = open( buf, O_RDWR );
	if ( fd < 0 )
		throw "Cannot access value";

	this->gpioFD = fd;

	cout<<"FD: "<<fd<<endl;

	return 0;
}

int SysfsInputDriver::ClosePort( int portnumber )
{
	this->gpio_unexport( portnumber );

	return 0;
}

void SysfsInputDriver::SetValue( int portnumber, int value )
{
	int setValueResult = this->gpio_set_value( portnumber, value );

	if ( setValueResult != 0 )
		perror( "Set GPIO Value fails to succeed." );
}

int SysfsInputDriver::GetValue( int portnumber )
{
	int value;
	int getValueResult = this->gpio_get_value( portnumber, &value );

	if ( getValueResult != 0 )
		perror( "Get GPIO Value fails to succeed." );

	return value;
}

/****************************************************************
 * gpio_export
 ****************************************************************/
int SysfsInputDriver::gpio_export( unsigned int gpio )
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open( SYSFS_GPIO_DIR "/export", O_WRONLY );
	if ( fd < 0 )
	{
		perror( "gpio/export" );
		return fd;
	}

	len = snprintf( buf, sizeof( buf ), "%d", gpio );
	write( fd, buf, len );
	close( fd );

	return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int SysfsInputDriver::gpio_unexport( unsigned int gpio )
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open( SYSFS_GPIO_DIR "/unexport", O_WRONLY );
	if ( fd < 0 )
	{
		perror( "gpio/export" );
		return fd;
	}

	len = snprintf( buf, sizeof( buf ), "%d", gpio );
	write( fd, buf, len );
	close( fd );
	return 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int SysfsInputDriver::gpio_set_dir( unsigned int gpio,
		unsigned int out_flag )
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf( buf, sizeof( buf ), SYSFS_GPIO_DIR "/gpio%d/direction",
			gpio );

	fd = open( buf, O_WRONLY );
	if ( fd < 0 )
	{
		perror( "gpio/direction" );
		return fd;
	}

	if ( out_flag )
		write( fd, "out", 4 );
	else
		write( fd, "in", 3 );

	close( fd );
	return 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int SysfsInputDriver::gpio_set_value( int gpio, int value )
{
	lseek( this->gpioFD, 0, SEEK_SET );

	if ( this->gpioFD < 0 )
	{
		perror( "gpio/set-value" );
		return this->gpioFD;
	}

	if ( value )
		write( this->gpioFD, "1", 2 );
	else
		write( this->gpioFD, "0", 2 );

	return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int SysfsInputDriver::gpio_get_value( int gpio, int *value )
{
	char ch;

	lseek( this->gpioFD, 0, SEEK_SET );

	if ( this->gpioFD < 0 )
	{
		perror( "gpio/get-value" );
		return this->gpioFD;
	}

	read( this->gpioFD, &ch, 1 );

	if ( ch != '0' )
	{
		*value = 1;
	}
	else
	{
		*value = 0;
	}

	return 0;
}

/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int SysfsInputDriver::gpio_set_edge( unsigned int gpio, char *edge )
{
//	int fd, len;
//	char buf[MAX_BUF];
//
//	len = snprintf( buf, sizeof( buf ), SYSFS_GPIO_DIR "/gpio%d/edge", gpio );
//
//	fd = open( buf, O_WRONLY );
//	if ( fd < 0 )
//	{
//		perror( "gpio/set-edge" );
//		return fd;
//	}
//
//	write( fd, edge, strlen( edge ) + 1 );
//	close( fd );
	return 0;
}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int SysfsInputDriver::gpio_fd_open( unsigned int gpio )
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf( buf, sizeof( buf ), SYSFS_GPIO_DIR "/gpio%d/uevent", gpio );

	fd = open( buf, O_RDONLY | O_NONBLOCK );
	if ( fd < 0 )
	{
		perror( "gpio/fd_open" );
	}
	return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int SysfsInputDriver::gpio_fd_close( int fd )
{
	return close( fd );
}

