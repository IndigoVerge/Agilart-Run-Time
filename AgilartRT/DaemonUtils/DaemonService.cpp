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
#include "DaemonService.h"
#include <iostream>
#include "Logging/EventLogger.h"

using namespace std;

DaemonService::DaemonService()
{
	this->isInitialized = false;
	this->isStarted = false;
}

DaemonService::~DaemonService()
{
}

int DaemonService::SetPidLock()
{
	const char *lock_file_name = Configuration::Instance()->GetStringParam( "LockFilePath" ).c_str();
	int lfp = open( lock_file_name, O_RDWR | O_CREAT, 0640 );
	int tryToLock;

	if ( lfp != -1 )
	{
		tryToLock = this->RegisterLock( lfp, F_SETLK, F_WRLCK, 0, SEEK_SET, 0 );

		if ( tryToLock == -1 )
		{
			cout << "Cannot lock the specified lock file!" << endl;
			exit( 1 );
		}

		if ( ftruncate( lfp, 0 ) == 0 )
		{
			char pidtext[1024];
			snprintf( pidtext, sizeof( pidtext ), "%ld\n", (long) getpid() );

			int r = write( lfp, pidtext, strlen( pidtext ) );
			if ( r == -1 )
			{
				cout << "Cannot write to the specified lock file!" << endl;
				exit( 1 );
			}
		}
	}

	return 0; //LOCK SUCCEEDED
}

int DaemonService::RegisterLock( int fd, int cmd, int type, off_t offset,
		int whence, off_t len )
{
	struct flock lock;

	lock.l_type = (short) type;
	lock.l_start = offset;
	lock.l_whence = (short) whence;
	lock.l_len = len;
	lock.l_pid = getpid();

	return fcntl( fd, cmd, &lock );
}

void DaemonService::Initialize()
{
	pid_t pid, sid;

	/* already a daemon */
	if ( getppid() == 1 )
		return;

	/* Create the lock file */
	this->SetPidLock();

	/* Drop user if there is one, and we were run as root */
	if ( getuid() == 0 || geteuid() == 0 )
	{
		const char* uid = Configuration::Instance()->GetStringParam( "Uid" ).c_str();
		struct passwd *pw = getpwnam( uid );
		if ( pw )
		{
			string uidString = uid;
			string s = "setting user to " + uidString;

			EventLogger::Instance()->WriteNotice( s.c_str() );
			setuid( pw->pw_uid );
		}
		else
		{
			cout
					<< "Error while setting the UID to the one spcified in the configuration!";
			exit( 1 );
		}
	}

	/* double-fork pattern to prevent zombie children */
	if ( ( pid = fork() ) < 0 )
	{
		EventLogger::Instance()->WriteError(
				"Deamon: Error Forking from the parent process" );
		exit( 1 );
	}
	else if ( pid != 0 )
	{
		/* parent */
		exit( 0 );
	}

	umask( 027 );

	sid = setsid(); /* get a new process group. */
	if ( sid < 0 )
	{
		EventLogger::Instance()->WriteError( "Deamon: setsid fatal error" );
		exit( 1 );
	}

	/* At this point we are executing as the child process */
	pid_t parent = getppid();

	/* Cancel certain signals */
	signal( SIGCHLD, SIG_DFL ); /* A child process dies */
	signal( SIGTSTP, SIG_IGN ); /* Various TTY signals */
	signal( SIGTTOU, SIG_IGN );
	signal( SIGTTIN, SIG_IGN );
	signal( SIGHUP, SIG_IGN ); /* Ignore hangup signal */
	signal( SIGTERM, SIG_DFL ); /* Die on SIGTERM */

	/* Change the current working directory.  This prevents the current
	 directory from being locked; hence not being able to remove it. */
	if ( ( chdir( "/" ) ) < 0 )
	{
		EventLogger::Instance()->WriteError( "Deamon: chdir fatal error" );
		// syslog( LOG_ERR, "unable to change directory to %s, code %d (%s)",
		//         "/", errno, strerror(errno) );
		exit( EXIT_FAILURE );
	}

	/* Redirect standard files to /dev/null */
	freopen( "/dev/null", "r", stdin );
	freopen( "/dev/null", "w", stdout );
	freopen( "/dev/null", "w", stderr );

	/* Tell the parent process that we are A-okay */
	kill( parent, SIGUSR1 );
}

void DaemonService::Start()
{
	while ( true )
	{
		; //Do some work;
	}
}

void DaemonService::Stop()
{
	/* Perform cleanup here */
	EventLogger::Instance()->WriteError( "Removing Deamon..." );

	/* only need to remove the lock file if we're daemonized... */
	const char *lockFileName = Configuration::Instance()->GetStringParam( "LockFilePath" ).c_str();

	remove( lockFileName );
	EventLogger::Instance()->WriteError( "Deamon lock file removed." );

	exit( 0 );
}
