#ifndef FILEEXISTSINPUTWATCHER_H
#define FILEEXISTSINPUTWATCHER_H

#include <string>
#include <thread>
#include <atomic>
#include <functional>

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

