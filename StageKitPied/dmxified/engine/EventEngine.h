#ifndef EVENTENGINE_H
#define EVENTENGINE_H

#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <algorithm> // For std::remove

#include "../config/ConfigHelpers.h"
#include "../config/MappingConfig.h"
#include "../config/MappingConfigConsts.h"
#include "../output/QlcplusOutputProcessor.h"
#include "../output/StageKitOutputProcessor.h"
#include "../../controller/RpiLightsController.h"
#include "../../stagekit/StageKitConsts.h"
#include "InputEvent.h"
#include "PrioritizedOutputOverride.h"


#ifdef DEBUG
  #define MSG_EVENTENGINE_DEBUG( str ) do { std::cout << std::this_thread::get_id() << "|EventEngine : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_EVENTENGINE_DEBUG( str ) do { } while ( false )
#endif

#define MSG_EVENTENGINE_ERROR( str ) do { std::cerr << std::this_thread::get_id() << "|EventEngine : ERROR : " << str << std::endl; } while( false )
#define MSG_EVENTENGINE_INFO( str ) do { std::cout << std::this_thread::get_id() << "|EventEngine : INFO : " << str << std::endl; } while( false )

// Forward declare FileExistsInputWatcher because the preprocessor will see the vector in private section before include
class FileExistsInputWatcher;


class EventEngine {
public:
	static long defaultNoEventIdleTimeMs;

	// Destructor since we are dealing with a Singleton
	~EventEngine();

	// Overloaded Singleton function to get EventEngine with default values for unspecified parameters.
	static EventEngine& getInstance(
    		const MappingConfig& mappings,
			RpiLightsController& rpiLightsController
	);

	// Singleton function to get EventEngine
    static EventEngine& getInstance(
    		const MappingConfig& mappings,
			RpiLightsController& rpiLightsController,
			const long noEventIdleTime,
			const std::string& websocketUrl,
			const long qlcplusConnectSleepTimeMs,
			const long qlcplusSendSleepTimeMs,
			const long fileExistsInputWatcherSleepTimeMs
	);

    static void handleTimeUpdate(long timePassed);

    // Delete the copy constructor and assignment operator to avoid multiple instances
    EventEngine(const EventEngine&) = delete;
    EventEngine& operator=(const EventEngine&) = delete;

    void handleInputEvent(const InputEvent& inputEvent);

private:
    // Private constructor
    EventEngine(
    		const MappingConfig& mappings,
			RpiLightsController& rpiLightsController,
			const long noEventIdleTime,
			const std::string& websocketUrl,
			const long qlcplusConnectSleepTimeMs,
			const long qlcplusSendSleepTimeMs,
			const long fileExistsInputWatcherSleepTimeMs
	);

    void doHandleInputEvent(const InputEvent& inputEvent);
    void queueInternalInputEvent(SKRUMBLEDATA eventType);
    void processQueuedEvent();
    void incrementTimePassedMs(long incrementBy);

    // Methods for handling overrides
    void override(const PrioritizedOutputOverride& override);
    void clearOverride(const PrioritizedOutputOverride& override);
    const bool isOverridden(const PrioritizedOutputOverride& override) const;

    std::string lastInputId;                                                       // Last processed input ID; used for deterring idle event spam
    SKRUMBLEDATA queuedEvent;
    std::vector<PrioritizedOutputOverride> outputOverrideList;
    MappingConfig mappings;                                                        // Configuration for number of watchers
    StageKitOutputProcessor skProcessor;
    QlcplusOutputProcessor qlcProcessor;                                           // Output processor
    std::vector<std::shared_ptr<FileExistsInputWatcher>> fileExistsInputWatchers;  // File watchers for fileInputExists input events.

    // Utility function since we are dealing with XML strings in the config
    const SKRUMBLEDATA stageKitOutputSubtypeToSkRumbleData(const Output& output);

    static EventEngine* instance; // Singleton instance
    static std::mutex mutex;      // Mutex to protect singleton initialization

    long time_passed_ms;
    long m_idletime_ms;
    long m_idletime_ms_count;
    bool m_idle_status_cleared;
};

#endif // EVENTENGINE_H
