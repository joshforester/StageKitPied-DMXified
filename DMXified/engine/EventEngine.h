#ifndef EVENTENGINE_H
#define EVENTENGINE_H

#ifdef DEBUG
  #define MSG_EVENTENGINE_DEBUG( str ) do { std::cout << "EventEngine : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_EVENTENGINE_DEBUG( str ) do { } while ( false )
#endif

#define MSG_EVENTENGINE_ERROR( str ) do { std::cout << "EventEngine : ERROR : " << str << std::endl; } while( false )
#define MSG_EVENTENGINE_INFO( str ) do { std::cout << "EventEngine : INFO : " << str << std::endl; } while( false )

// Forward declare FileExistsInputWatcher because the preprocessor will see the vector in private section before include
class FileExistsInputWatcher;

#include <vector>
#include <string>
#include <mutex>
#include <algorithm> // For std::remove

#include "../config/ConfigHelpers.h"
#include "../config/MappingConfig.h"
#include "../config/MappingConfigConsts.h"
#include "../output/StageKitOutputProcessor.h"
#include "../output/QlcplusOutputProcessor.h"
#include "../../StageKitPied/stagekit/StageKitConsts.h"
#include "InputEvent.h"
#include "PrioritizedOutputOverride.h"

class EventEngine {
public:
	// Destructor since we are dealing with a Singleton
	~EventEngine();

	// Constructor that takes a MappingConfig
    static EventEngine& getInstance(
    		const MappingConfig& mappings,
			RpiLightsController& rpiLightsController,
			const std::string& url,
			const unsigned int fileExistsInputWatcherSleepTimeMs
	);

    // Delete the copy constructor and assignment operator to avoid multiple instances
    EventEngine(const EventEngine&) = delete;
    EventEngine& operator=(const EventEngine&) = delete;

    // Methods for handling overrides
    void handleInputEvent(const InputEvent& inputEvent);
    void override(const PrioritizedOutputOverride& override);
    void clearOverride(const PrioritizedOutputOverride& override);
    const bool isOverridden(const PrioritizedOutputOverride& override) const;

private:
    // Private constructor
    EventEngine(
    		const MappingConfig& mappings,
			RpiLightsController& rpiLightsController,
			const std::string& url,
			const unsigned int fileExistsInputWatcherSleepTimeMs
	);

    std::string lastInputId;                                                       // Last processed input ID; used for deterring SK_FOG_OFF spam
    std::vector<PrioritizedOutputOverride> outputOverrideList;
    MappingConfig mappings;                                                        // Configuration for number of watchers
    StageKitOutputProcessor skProcessor;
    QlcplusOutputProcessor qlcProcessor;                                           // Output processor
    std::vector<std::shared_ptr<FileExistsInputWatcher>> fileExistsInputWatchers;  // File watchers for fileInputExists input events.

    // Utility function since we are dealing with XML strings in the config
    const SKRUMBLEDATA stageKitOutputSubtypeToSkRumbleData(const Output& output);

    static EventEngine* instance; // Singleton instance
    static std::mutex mutex;      // Mutex to protect singleton initialization
};

#endif // EVENTENGINE_H
