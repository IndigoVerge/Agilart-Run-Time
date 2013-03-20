#include <iostream>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <map>
#include <list>
#include <stdlib.h>
#include "BaseDriver.h"
#include "Command.h"
#include "CommandParameter.h"
#include "Consts.h"
#include <list>
#include "DriverCommand.h"

#define CONFIG_Drivers_DIR "drivers"
#define DRIVERS_ENTYPOINT_SMBNAME "CreateDrivers"
#define PATH_PARAM "Path"

using namespace std;

int main(int argc, char* argv[])
{

  string driversLocation = "/home/pi/ZWaveDrivers/Debug/libZWaveDrivers.so"; // use "man dlopen" to check where to put the file
  list<BaseDriver*> driversList;

  void* driverLib = dlopen( driversLocation.c_str(), RTLD_LAZY );

  if ( !driverLib )
  {
    char *error;
    error = dlerror();

    if ( error != 0 )
      cout << "Error: " << endl << error << endl;
    return 0;
  }

  create_drivers* create = (create_drivers*) dlsym( driverLib, "CreateDrivers" );

  if ( !create )
  {
    char *error;
    error = dlerror();

    if ( error != 0 )
      cout << "Error: " << endl << error << endl;
    return 0;
  }

  driversList = create();
  list<BaseDriver*>::iterator it;
  string* path = new string("/dev/ttyUSB0");
  int* gpio = new int(26);
  CommandParameter* gpioParam = new CommandParameter(GPIO_NUMBER_PARAM, (void*)gpio, INT_TYPE);
  CommandParameter* pathParam = new CommandParameter(PATH_PARAM, (void*)path, STRING_TYPE);
  int param = atoi(argv[1]);
  int* value = new int(param);
  CommandParameter* valParam = new CommandParameter(VALUE_KEY, (void*)value, INT_TYPE);

  Command initCmd = Command(INIT_CMD);
  initCmd.AddParameter(pathParam);

  Command setCmd = Command(SETVAL_CMD);
  setCmd.AddParameter(valParam);

  Command stopCmd = Command(STOP_CMD);
  cout<<initCmd.Name()<<endl;

  BaseDriver* zWave;

  for ( it = driversList.begin(); it != driversList.end(); it++ )
  {
    cout<<"Driver: "<<( *it )->Name()<<endl;
    zWave = (*it)->Clone();
    break;
  }

  cout<<"Clone name: "<<zWave->Name()<<endl;
  zWave->ExecuteCommand(initCmd);
  cout<<"Init executed!\n";

  zWave->ExecuteCommand(setCmd);

  zWave->ExecuteCommand(stopCmd);
  cout<<"Stop Executed\n";


  return 0;
}
