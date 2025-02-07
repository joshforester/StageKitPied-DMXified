#include "FileExistsInputWatcher.h"


unsigned int FileExistsInputWatcher::defaultFileExistsInputWatcherSleepTimeMs = 100;

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
            MSG_FILEEXISTSINPUTWATCHER_DEBUG( "FileExistsInputWatcher thread joined successfully." );
        }
    } catch (const std::exception& e) {
        MSG_FILEEXISTSINPUTWATCHER_ERROR( "Error during FileExistsInputWatcher cleanup: " << e.what() );
    } catch (...) {
        MSG_FILEEXISTSINPUTWATCHER_ERROR( "Unknown error during FileExistsInputWatcher cleanup." );
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
        	MSG_FILEEXISTSINPUTWATCHER_ERROR( "Exception in watchFile thread: " << e.what() );
        } catch (...) {
        	MSG_FILEEXISTSINPUTWATCHER_ERROR( "Unknown exception in watchFile thread." );
        }
    }
}

bool FileExistsInputWatcher::fileExists() const {
    std::ifstream file(filePath);
    return file.good();
}
