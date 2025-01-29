#include "FileExistsInputWatcher.h"

#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include "EventEngine.h"
#include "InputEvent.h"

FileExistsInputWatcher::FileExistsInputWatcher(std::string id, const std::string& filePath, unsigned int sleepTimeMs, EventEngine& engine)
    : id(id), filePath(filePath), sleepTimeMs(sleepTimeMs), engine(engine), isRunning(true), previousExistence(false) {
    watcherThread = std::thread(&FileExistsInputWatcher::watchFile, this);
}

FileExistsInputWatcher::~FileExistsInputWatcher() {
    stop();
    if (watcherThread.joinable()) {
        watcherThread.join();
    }
}

void FileExistsInputWatcher::stop() {
    isRunning = false;
}

void FileExistsInputWatcher::watchFile() {
    while (isRunning) {
        bool currentExistence = fileExists();

        // If the file existence state has changed, invoke the appropriate method on the EventEngine
        if (currentExistence != previousExistence) {
            if (currentExistence) {
            	InputEvent inputEvent(id);
                engine.handleInputEvent(inputEvent);  // Notify the engine that the file now exists
            } else {
            	InputEvent inputEvent(id + "Off");
                engine.handleInputEvent(inputEvent);  // Notify the engine that the file no longer exists
            }
            previousExistence = currentExistence;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));
    }
}

bool FileExistsInputWatcher::fileExists() const {
    std::ifstream file(filePath);
    return file.good();
}
