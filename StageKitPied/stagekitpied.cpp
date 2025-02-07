#include <thread>
#include <csignal>
#include <iostream>
#include <unistd.h>
#include <atomic>
#include <stdio.h>
#include <cstring>

#include "helpers/SleepTimer.h"
#include "controller/RpiLightsController.h"

#define INI_FILE "lights.ini"

#define MSG_SKP_INFO( str ) do { std::cout << std::this_thread::get_id() << "|StageKitPied : INFO : " << str << std::endl; } while( false )
#define MSG_SKP_ERROR( str ) do { std::cerr << std::this_thread::get_id() << "|StageKitPied : ERROR : " << str << std::endl; } while( false )

// *******
// sigterm/sigint
// *******
std::atomic<bool> running(true);  // Atomic flag to control the running state

void signalHandler(int signum) {
    MSG_SKP_INFO( "Signal (" << signum << ") received. Exiting gracefully..." );
    running.store(false);  // Set the flag to false to stop the program loop
}


// ****
// main
// ****

// Stage Kit Pied

int main(int arc, char *argv[]) {

  // Set up signal handling for SIGINT (Ctrl+C) and SIGTERM (systemctl stop)
  signal(SIGINT, signalHandler);  // For Ctrl+C
  signal(SIGTERM, signalHandler); // For systemctl stop

  int pid = getpid();

  MSG_SKP_INFO( "Program launched with PID = " << pid );

  RpiLightsController lightsController( INI_FILE );
  while( running.load() && !lightsController.Start() ) {
    MSG_SKP_ERROR( "Unable to start.");
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  MSG_SKP_INFO( "Started." );

  // Sleepy timer
  long sleepyTime = 0;

  SleepTimer sleeper;
  sleeper.SetSleepTimeMax( sleepyTime );
  sleeper.Start();

  long slept_ms = 10;
  long sleep_time_new = 10;

  // Main loop
  while( running.load() ) {

    sleep_time_new = lightsController.Update( slept_ms );

    sleeper.SetSleepTimeMax( sleep_time_new );
    slept_ms = sleeper.Sleep();

  }

  lightsController.Stop();

  MSG_SKP_INFO( "Program ended." );

  return 0;
}
