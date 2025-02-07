#ifndef FILEEXISTSINPUTWATCHER_H
#define FILEEXISTSINPUTWATCHER_H

#include <string>
#include <atomic>
#include <functional>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include "EventEngine.h"
#include "InputEvent.h"

#ifdef DEBUG
  #define MSG_FILEEXISTSINPUTWATCHER_DEBUG( str ) do { std::cout << std::this_thread::get_id() << "|FileExistsInputWatcher : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_FILEEXISTSINPUTWATCHER_DEBUG( str ) do { } while ( false )
#endif

#define MSG_FILEEXISTSINPUTWATCHER_ERROR( str ) do { std::cerr << std::this_thread::get_id() << "|FileExistsInputWatcher : ERROR : " << str << std::endl; } while( false )
#define MSG_FILEEXISTSINPUTWATCHER_INFO( str ) do { std::cout << std::this_thread::get_id() << "|FileExistsInputWatcher : INFO : " << str << std::endl; } while( false )


class EventEngine;  // Forward declaration

class FileExistsInputWatcher {
public:
	static unsigned int defaultFileExistsInputWatcherSleepTimeMs;

    FileExistsInputWatcher(std::string id, const std::string& filePath, EventEngine& engine, unsigned int sleepTimeMs = defaultFileExistsInputWatcherSleepTimeMs);
    ~FileExistsInputWatcher();

    void stop();

    // Delete the copy constructor and copy assignment operator
    FileExistsInputWatcher(const FileExistsInputWatcher&) = delete;
    FileExistsInputWatcher& operator=(const FileExistsInputWatcher&) = delete;

private:
    void watchFile();
    bool fileExists() const;

    std::string id;
    std::string filePath;
    EventEngine& engine;
    unsigned int sleepTimeMs;
    std::thread watcherThread;
    std::atomic<bool> isRunning;
    bool previousExistence;
};

#endif // FILEEXISTSINPUTWATCHER_H

