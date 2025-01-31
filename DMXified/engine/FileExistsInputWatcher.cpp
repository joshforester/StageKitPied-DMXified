#include "FileExistsInputWatcher.h"

FileExistsInputWatcher::FileExistsInputWatcher(std::string id, const std::string& filePath, EventEngine& engine, unsigned int sleepTimeMs)
    : id(id), filePath(filePath), engine(engine), sleepTimeMs(sleepTimeMs), isRunning(true), previousExistence(false) {
    watcherThread = std::thread(&FileExistsInputWatcher::watchFile, this);
}

FileExistsInputWatcher::~FileExistsInputWatcher() {
    try {
        stop();  // Stop the thread

        // Only attempt to join the thread if it is joinable
        if (watcherThread.joinable()) {
            watcherThread.join();  // Safely join the thread
            std::cout << "FileExistsInputWatcher thread joined successfully." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error during FileExistsInputWatcher cleanup: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error during FileExistsInputWatcher cleanup." << std::endl;
    }
}

void FileExistsInputWatcher::stop() {
    isRunning = false;
}

void FileExistsInputWatcher::watchFile() {
    while (isRunning) {
        try {
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
        } catch (const std::exception& e) {
            std::cerr << "Exception in watchFile thread: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception in watchFile thread." << std::endl;
        }
    }
}

bool FileExistsInputWatcher::fileExists() const {
    std::ifstream file(filePath);
    return file.good();
}
