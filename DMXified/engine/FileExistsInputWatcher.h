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

class EventEngine;  // Forward declaration

class FileExistsInputWatcher {
public:
    FileExistsInputWatcher(std::string id, const std::string& filePath, EventEngine& engine, unsigned int sleepTimeMs = 100);
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

