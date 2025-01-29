#ifndef EVENTENGINE_H
#define EVENTENGINE_H

#ifdef DEBUG
  #define MSG_EVENTENGINE_DEBUG( str ) do { std::cout << "EventEngine : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_EVENTENGINE_DEBUG( str ) do { } while ( false )
#endif

#define MSG_EVENTENGINE_ERROR( str ) do { std::cout << "EventEngine : ERROR : " << str << std::endl; } while( false )
#define MSG_EVENTENGINE_INFO( str ) do { std::cout << "EventEngine : INFO : " << str << std::endl; } while( false )


#include <vector>
#include <string>
#include <algorithm> // For std::remove

#include "../config/ConfigHelpers.h"
#include "../config/MappingConfig.h"
#include "../config/MappingConfigConsts.h"
#include "../../StageKitPied/stagekit/StageKitConsts.h"
#include "InputEvent.h"
#include "PrioritizedOutputOverride.h"
#include "StageKitOutputProcessor.h"
#include "QlcplusOutputProcessor.h"
#include "FileExistsInputWatcher.h"

class EventEngine {
public:
    // Constructor that takes a MappingConfig
    EventEngine(const MappingConfig& mappings, RpiLightsController& rpiLightsController);

    // Methods for handling overrides
    void handleInputEvent(const InputEvent& inputEvent);
    void override(const PrioritizedOutputOverride& override);
    void clearOverride(const PrioritizedOutputOverride& override);
    const bool isOverridden(const PrioritizedOutputOverride& override) const;

private:
    std::string lastInputId;                                            // Last processed input ID; used for deterring SK_FOG_OFF spam
    std::vector<PrioritizedOutputOverride> outputOverrideList;
    MappingConfig mappings;                                             // Configuration for number of watchers
    StageKitOutputProcessor skProcessor;
    QlcplusOutputProcessor* qlcProcessor;                               // Output processor (placeholder)

    // Utility function since we are dealing with XML strings in the config
    const SKRUMBLEDATA stageKitOutputSubtypeToSkRumbleData(const Output& output);
};

#endif // EVENTENGINE_H
