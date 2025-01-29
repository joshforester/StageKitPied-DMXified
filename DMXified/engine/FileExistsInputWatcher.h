#ifndef FILEEXISTSINPUTWATCHER_H
#define FILEEXISTSINPUTWATCHER_H

#include <string>
#include <thread>
#include <atomic>
#include <functional>

class EventEngine;  // Forward declaration

class FileExistsInputWatcher {
public:
    FileExistsInputWatcher(std::string id, const std::string& filePath, unsigned int sleepTimeMs, EventEngine& engine);
    ~FileExistsInputWatcher();

    void stop();

private:
    void watchFile();
    bool fileExists() const;

    std::string id;
    std::string filePath;
    unsigned int sleepTimeMs;
    EventEngine& engine;
    std::thread watcherThread;
    std::atomic<bool> isRunning;
    bool previousExistence;
};

#endif // FILEEXISTSINPUTWATCHER_H

