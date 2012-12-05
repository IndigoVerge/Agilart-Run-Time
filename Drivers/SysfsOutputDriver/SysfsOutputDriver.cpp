/*
 * SysfsOutputDriver.cpp
 *
 *  Created on: Nov 27, 2012
 *      Author: developer
 */

#include "SysfsOutputDriver.h"
#define SysfsOutputDriver_NAME  "Sysfs Output Driver";
#define SysfsOutputDriver_TYPE  "SysfsOutput";
#define SysfsOutputDriver_VERSION "0.0.1"
#define SysfsOutputDriver_DESCR "Sysfs Output Description"
#define SysfsOutputDriver_PUBL "CompletIT Ltd."

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64

SysfsOutputDriver::SysfsOutputDriver() : BaseDriver()
{
	Command initCommand( INIT_CMD );
	initCommand.AddParameter( new CommandParameter( PORTNUMBER_KEY, NULL, "int" ) );

	Command setValueCommand( SETVAL_CMD );
	setValueCommand.AddParameter( new CommandParameter( VALUE_KEY, NULL, "int" ) );

	this->commands->push_back( initCommand );
	this->commands->push_back( setValueCommand );

	this->supportedComands->push_back( initCommand );
	this->supportedComands->push_back( setValueCommand );

	Command stopCommand( STOP_CMD );
	this->supportedComands->push_back( stopCommand );
}

SysfsOutputDriver::~SysfsOutputDriver()
{
	// implemented in the BaseDriver
}

const char* SysfsOutputDriver::Type()
{
	return SysfsOutputDriver_TYPE;
}

const char* SysfsOutputDriver::Version()
{
	return SysfsOutputDriver_VERSION;
}

const char* SysfsOutputDriver::Name()
{
	return SysfsOutputDriver_NAME;
}

const char* SysfsOutputDriver::Description()
{
	return SysfsOutputDriver_DESCR;
}

const char* SysfsOutputDriver::Publisher()
{
	return SysfsOutputDriver_PUBL;
}

int SysfsOutputDriver::GetStatus()
{
	return this->status;
}

void SysfsOutputDriver::SetStatus( int value )
{
	this->status = value;
}

int SysfsOutputDriver::GetPortNumber()
{
	return this->portNumber;
}

int SysfsOutputDriver::GetGpioValue()
{
	int newStatus;
	int statusResult = this->gpio_get_value( this->portNumber, &newStatus );

	if ( statusResult != 0 )
	{
		cout << "Error in Sysfs GPIO get value\n";
		throw "Error in Sysfs GPIO get value";
	}

	return statusResult;
}

void SysfsOutputDriver::ExecuteCommand( Command command )
{
	string logMessage = "SysfsGenericGPIODriver - Execute Command " + command.Name();
	EventLogger::Instance()->WriteInformation( logMessage.c_str() );

	if ( !strcmp( command.Name().c_str(), INIT_CMD ) )
	{
		string direction;
		int outFlag = 1;

		CommandParameter* portnumberParam =	command.GetParameter( PORTNUMBER_KEY );

		if ( this->portNumber != 0 )
			this->ClosePort( this->portNumber );

		this->portNumber = *(int*) ( portnumberParam->Value );
		this->shouldStop = false;
		this->OpenPort( this->portNumber, outFlag );
	}
	else if ( !strcmp( command.Name().c_str(), SETVAL_CMD ) )
	{
		CommandParameter* valueParam = command.GetParameter( VALUE_KEY );
		int value = *(int*) ( valueParam->Value );

		EventLogger::Instance()->WriteVerbose("SYSFS - Set Value - port %d  value - %d ", this->portNumber, value);

		if ( ( value == 0 || value == 1 ) && this->portNumber != 0 )
			this->SetValue( this->portNumber, value );
	}
	else if ( !strcmp( command.Name().c_str(), STOP_CMD ) )
	{
		this->ClosePort( this->portNumber );
		EventLogger::Instance()->WriteVerbose("SysfsGenericGPIODriver - Stop thread succeeded. ");
	}
}

list<Command> SysfsOutputDriver::Commands()
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

BaseDriver* SysfsOutputDriver::Clone()
{
	BaseDriver* clone = new SysfsOutputDriver();
	return clone;
}

Command* SysfsOutputDriver::GetCommand( const char* name )
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

int SysfsOutputDriver::OpenPort( int portnumber, int isOutFlag )
{
	int exportResult = this->gpio_export( portnumber );

	if ( exportResult < 0 )
		throw "Cannot export gpio";

	exportResult = this->gpio_set_dir( portnumber, isOutFlag );

	if ( exportResult < 0 )
		throw "Cannot set gpio direction";

	int fd, len;
	char buf[MAX_BUF];

	len = snprintf( buf, sizeof( buf ), SYSFS_GPIO_DIR "/gpio%d/value",
			portnumber );

	fd = open( buf, O_RDWR );
	if ( fd < 0 )
		throw "Cannot access value";

	this->gpioFD = fd;

	return 0;
}

int SysfsOutputDriver::ClosePort( int portnumber )
{
	this->gpio_unexport( portnumber );

	return 0;
}

void SysfsOutputDriver::SetValue( int portnumber, int value )
{
	int setValueResult = this->gpio_set_value( portnumber, value );

	if ( setValueResult != 0 )
		perror( "Set GPIO Value fails to succeed." );
}

int SysfsOutputDriver::GetValue( int portnumber )
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
int SysfsOutputDriver::gpio_export( unsigned int gpio )
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
int SysfsOutputDriver::gpio_unexport( unsigned int gpio )
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
int SysfsOutputDriver::gpio_set_dir( unsigned int gpio,
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
int SysfsOutputDriver::gpio_set_value( int gpio, int value )
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
int SysfsOutputDriver::gpio_get_value( int gpio, int *value )
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

int SysfsOutputDriver::gpio_set_edge( unsigned int gpio, char *edge )
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

int SysfsOutputDriver::gpio_fd_open( unsigned int gpio )
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

int SysfsOutputDriver::gpio_fd_close( int fd )
{
	return close( fd );
}


