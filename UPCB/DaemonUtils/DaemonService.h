#ifndef DAEMONSERVICE_H_
#define DAEMONSERVICE_H_

#include "../Common/Configuration.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

class DaemonService
{
	private:

		bool isInitialized;
		bool isStarted;

		int RegisterLock( int, int, int, off_t, int, off_t );
		int SetPidLock();

	public:
		DaemonService();
		virtual ~DaemonService();

		void Initialize();
		void Start();
		void Stop();
};

#endif /* DAEMONSERVICE_H_ */
