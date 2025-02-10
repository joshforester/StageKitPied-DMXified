#include "EventEngine.h"
#include "FileExistsInputWatcher.h"

// Initialize the static members
EventEngine* EventEngine::instance = nullptr;
std::mutex EventEngine::mutex;
long EventEngine::defaultNoEventIdleTimeMs = 20;


// Constructor
EventEngine::EventEngine(
		const MappingConfig& mappings,
		RpiLightsController& rpiLightsController,
		const long noEventIdleTime,
		const std::string& websocketUrl,
		const long qlcplusConnectSleepTimeMs,
		const long qlcplusSendSleepTimeMs,
		const long fileExistsInputWatcherSleepTimeMs
)
    : mappings(mappings),
	  skProcessor(rpiLightsController),
	  qlcProcessor(websocketUrl, qlcplusConnectSleepTimeMs, qlcplusSendSleepTimeMs) {

	lastInputId = skRumbleDataTypeToString(SKRUMBLEDATA::SK_ALL_OFF);

	queuedEvent = SKRUMBLEDATA::SK_NONE;

	time_passed_ms = 0;

	m_idletime_ms = noEventIdleTime;
	m_idletime_ms_count = 0;
	m_idle_status_cleared = true;


    const std::vector<Input> fileExistsInputs = mappings.getFileExistsInputs();
    for (const auto& fileExistsInput : fileExistsInputs) {
        // Use shared_ptr to store in the vector
        auto fileExistsInputWatcher = std::make_shared<FileExistsInputWatcher>(
            fileExistsInput.getId(),
			fileExistsInput.getFile(),
			*this,
			fileExistsInputWatcherSleepTimeMs
        );
        fileExistsInputWatchers.push_back(fileExistsInputWatcher);
    }

}

EventEngine::~EventEngine() {
	MSG_EVENTENGINE_INFO( "Cleaning up EventEngine..." );

    // Manually delete the static instance if necessary (only if you don't rely on shared_ptr cleanup)
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;  // Set to nullptr to avoid double-delete or invalid access
    }

    // Optionally, explicitly cleanup resources if necessary
    // For example, stopping background tasks or releasing system resources
    for (auto& watcher : fileExistsInputWatchers) {
        watcher.reset();  // Ensure shared_ptr releases its memory, even though it's redundant here
    }

    // No need to call any explicit cleanup for skProcessor and qlcProcessor, as their destructors
    // will automatically clean up the necessary resources when EventEngine is destroyed
}

EventEngine& EventEngine::getInstance(
		const MappingConfig& mappings,
		RpiLightsController& rpiLightsController
) {
	return EventEngine::getInstance(
			mappings,
			rpiLightsController,
			EventEngine::defaultNoEventIdleTimeMs,
			"",
			0,
			0,
			0
		);
}


// Singleton getInstance method
EventEngine& EventEngine::getInstance(
		const MappingConfig& mappings,
		RpiLightsController& rpiLightsController,
		const long noEventIdleTimeMs,
		const std::string& websocketUrl,
		const long qlcplusConnectSleepTimeMs,
		const long qlcplusSendSleepTimeMs,
		const long fileExistsInputWatcherSleepTimeMs
) {
    std::lock_guard<std::mutex> lock(mutex);  // Ensure thread safety during initialization
    if (instance == nullptr) {
    	std::string url = (websocketUrl.empty())
    					 ? QlcplusOutputProcessor::defaultQlcplusWebsocketUrl
    					 : websocketUrl;

        // If no value for feiw_sleepTimeMs is provided, use the default from FileExistsInputWatcher
        long feiw_sleepTimeMs = (fileExistsInputWatcherSleepTimeMs == 0)
                               ? FileExistsInputWatcher::defaultFileExistsInputWatcherSleepTimeMs
                               : fileExistsInputWatcherSleepTimeMs;

        instance = new EventEngine(
			mappings,
			rpiLightsController,
			noEventIdleTimeMs,
			url,
			qlcplusConnectSleepTimeMs,
			qlcplusSendSleepTimeMs,
			feiw_sleepTimeMs
		);
    }
    return *instance;
}

void EventEngine::incrementTimePassedMs(long incrementBy) {
	time_passed_ms += incrementBy;
}


void EventEngine::handleTimeUpdate(long timePassed) {
	if (instance != nullptr) {
		instance->incrementTimePassedMs(timePassed);
	}
}

void EventEngine::handleInputEvent(const InputEvent& inputEvent) {
	std::lock_guard<std::mutex> lock(mutex); // protect the modification of overrides, outputsToProcess, outputOverrideList, and lastInputId from FileExistsInputWatcher

	if (instance != nullptr) {
		this->doHandleInputEvent(inputEvent);
	}
}

// Handle an InputEvent.  Algorithm:
//  inputId is passed in:
//    look up input type and priority
//    if inputId is SK_FOG_OFF event and that was the last event processed:
//      ignore
//    else if outputType is overridden by inputId with higher inputPriority:
//      ignore
//    else:
//      process outputs for input
//      setup any override lists
//      set lastInputId
void EventEngine::doHandleInputEvent(const InputEvent& inputEvent) {

	try {

		const std::string inputId = inputEvent.getId();
		Input input = mappings.getInputById(inputId);

		MSG_EVENTENGINE_DEBUG("handleInputEvent called; current event[last event]: " << inputId << "[" << lastInputId << "].");

		// Handle idle event
		bool isIdleEvent = isIdleSkRumbleDataString(inputId);
		if (isIdleEvent) {

			// receiving idle event; only start counting again if idle status was cleared recently
			if ( m_idle_status_cleared ) { // ensure not already in idle status so we don't send duplicate IDLE_ON events
				if( m_idletime_ms > 0 ) {
					m_idletime_ms_count += time_passed_ms;
					time_passed_ms = 0;
					MSG_EVENTENGINE_DEBUG("m_idletime_ms[m_idletime_ms_count]:" + std::to_string(m_idletime_ms) + "[" + std::to_string(m_idletime_ms_count) + "]");
					if( m_idletime_ms_count > m_idletime_ms ) {
						m_idle_status_cleared = false;
						MSG_EVENTENGINE_DEBUG( "Timeout for idle time exceeded.  Sending IDLE_ON.");
						this->queueInternalInputEvent(SKRUMBLEDATA::IDLE_ON);
					}
				}
			}

			// ignore duplicate idle events:  SK_FOG_OFF-SK_FOG_OFF, IDLE_ON-IDLE_ON, IDLE_OFF-IDLE_OFF, SERIAL_RESTART-SERIAL_RESTART
			if (isIdleSkRumbleDataString(lastInputId) &&
			    ((inputId == lastInputId) || (inputId == skRumbleDataTypeToString(SKRUMBLEDATA::SK_FOG_OFF)))) {
				if (inputId != lastInputId) {
					lastInputId = inputId;
				}

				MSG_EVENTENGINE_DEBUG("Ignoring SK_FOG_OFF/idle event spam.");
				this->processQueuedEvent();
				return;
			}

		} else { // non-idle event

			// non-idel event, so reset count
			time_passed_ms = 0;
			m_idletime_ms_count = 0;

			// first non-idle event; clear idle status
			if (!m_idle_status_cleared) {
				m_idle_status_cleared = true;
				MSG_EVENTENGINE_DEBUG( "First non-idle event; resetting idle time and sending IDLE_OFF.");
				this->queueInternalInputEvent(SKRUMBLEDATA::IDLE_OFF);
			}

		}

		// If this is a negation event, populate it accordingly.
		const bool isNegationEvent = input.isNegationEvent();
		if (isNegationEvent) {
			MSG_EVENTENGINE_DEBUG( "Determined to be a negation event." );
			const Input negatedEvent = mappings.getInputById(input.getNegatedInputId());
			input.populateFromNegatedInput(negatedEvent);
		}

		// Let's now find the outputs that are not overridden.
		// First, get the priority from the Input
		const int priority = input.getPriority();

		// Get the list of outputs from MappingConfig based on input's ID
		const std::vector<Output> outputs = mappings.getOutputsByInputId(input.getId());
		std::vector<Output> outputsToProcess;
		std::vector<PrioritizedOutputOverride> overridesToProcess;

		// For each Output, create a PrioritizedOutputOverride from input priority, then use it
		// to check if the output should be processed.
		for (const Output& output : outputs) {
			const PrioritizedOutputOverride overrideToCheck(output.getType(), output.getSubtype(), priority);

			// Add it to the list to outputs to process if not already overridden
			if (!this->isOverridden(overrideToCheck)) {
				MSG_EVENTENGINE_DEBUG( "Output " + output.getType() + ":" + output.getSubtype() + "(P:" + std::to_string(priority) + ") determined to NOT be overridden." );
				outputsToProcess.push_back(output);
			} else {
				MSG_EVENTENGINE_DEBUG( "Output " + output.getType() + ":" + output.getSubtype() + "(P:" + std::to_string(priority) + ") determined to be overridden." );
			}
		}

		// WARNING:
		// This may cause confusion/problems where input priorities/overrides are complex,
		// since overrides are associated with inputs but act on outputs.
		// For now, I'm looking to service the simple use cases, and don't expect this to rear its head.
		//
		// If there are outputs to process, then the input event's outputs are not FULLY overridden.
		// So, we'll go ahead and process the input event's overrides.
		if (outputsToProcess.size() > 0) {
			for (const OutputOverride& overrideToProcess : input.getOutputOverrideList()) {
				overridesToProcess.push_back(
					PrioritizedOutputOverride(overrideToProcess.getType(), overrideToProcess.getSubtype(), input.getPriority())
				);
			}
		}

		// Time to process the outputs
		bool isFirst = true;
		for (const Output& outputToProcess : outputsToProcess) {
			if (isFirst) {
				std::cout << "\n";
				isFirst = false;
			}
			const std::string message = "Processing output " + outputToProcess.getType() + ":" + outputToProcess.getSubtype();
			MSG_EVENTENGINE_DEBUG(message);

			const OUTPUT_TYPE type = stringToOutputType(outputToProcess.getType());
			switch (type) {
				case stageKitOutput:
					MSG_EVENTENGINE_DEBUG( "Determined StageKitProcessor should process." );
					if (inputEvent.getRightWeight() != 0) {
						MSG_EVENTENGINE_DEBUG( "right_weight sent with input event; determined to be RB3 input." );
						skProcessor.process(
								outputToProcess,
								inputEvent.getLeftWeight(),
								inputEvent.getRightWeight()
						);
					} else {
						MSG_EVENTENGINE_DEBUG( "right_weight NOT sent with input event; will need to determine event from config." );
						SKRUMBLEDATA right_weight = this->stageKitOutputSubtypeToSkRumbleData(outputToProcess);
						skProcessor.process(
								outputToProcess,
								inputEvent.getLeftWeight(),
								right_weight
						);
					}
					break;
				case qlcplusOutput:
					try {
						MSG_EVENTENGINE_DEBUG("Determined QlcplusProcessor should process.");
						qlcProcessor.process(outputToProcess);
					} catch (const std::exception& e) {
						MSG_EVENTENGINE_ERROR("Error processing Qlcplus output: " << e.what());
					} catch (...) {
						MSG_EVENTENGINE_ERROR("Unknown error processing Qlcplus output.");
					}
					break;
				default:
					MSG_EVENTENGINE_ERROR("Unknown output type!");
					break;
			}


		    // If this is a negation event, clear the overrides, else add them.
		    for (const PrioritizedOutputOverride& overrideToProcess : overridesToProcess) {
		    	if (isNegationEvent) {
		            MSG_EVENTENGINE_DEBUG( "Clearing override " + overrideToProcess.getType() + ":" + overrideToProcess.getSubtype() + "(P:" + std::to_string(overrideToProcess.getPriority()) + ")." );
		    		this->clearOverride(overrideToProcess);
		    	} else {
		            MSG_EVENTENGINE_DEBUG( "Creating override " + overrideToProcess.getType() + ":" + overrideToProcess.getSubtype() + "(P:" + std::to_string(overrideToProcess.getPriority()) + ")." );
		        	this->override(overrideToProcess);
		    	}
		    }

		}

	    // Only doing this to prevent processing of SK_FOG_OFF spam.
	    lastInputId = inputId;  // Save inputId for comparison with SK_FOG_OFF.

		this->processQueuedEvent();

	} catch (const std::exception& e) {
		MSG_EVENTENGINE_ERROR("Exception in handleInputEvent: ");
	} catch (...) {
		MSG_EVENTENGINE_ERROR("Unknown exception in handleInputEvent.");
	}
}

void EventEngine::queueInternalInputEvent(SKRUMBLEDATA eventType) {
	queuedEvent = eventType;
}

void EventEngine::processQueuedEvent() {

	if (queuedEvent != SKRUMBLEDATA::SK_NONE) {

		InputEvent event(
			skRumbleDataTypeToString(queuedEvent),
			SKRUMBLEDATA::SK_NONE,
			queuedEvent
		);

		queuedEvent = SKRUMBLEDATA::SK_NONE; // clear queue
		this->doHandleInputEvent(event);

	}

}

// Override an existing output override
void EventEngine::override(const PrioritizedOutputOverride& override) {
    outputOverrideList.push_back(override);
}

// Clear an existing output override
void EventEngine::clearOverride(const PrioritizedOutputOverride& override) {
    auto it = std::remove_if(outputOverrideList.begin(), outputOverrideList.end(),
                             [&override](const PrioritizedOutputOverride& existingOverride) {
                                 return existingOverride == override;
                             });

    // Clear an existing output override
    if (it != outputOverrideList.end()) {
        outputOverrideList.erase(it, outputOverrideList.end());
    }
}

// Check if the override exists with a higher priority
const bool EventEngine::isOverridden(const PrioritizedOutputOverride& override) const {
    for (const auto& existingOverride : outputOverrideList) {
        if (existingOverride == override && existingOverride.getPriority() > override.getPriority()) {
            return true; // The override is present with a higher priority
        }
    }
    return false;
}

const SKRUMBLEDATA EventEngine::stageKitOutputSubtypeToSkRumbleData(const Output& output) {
	// Declare the variables outside of the switch statement
	int strobeSpeed = 0;
    bool fogOn = false;

    STAGEKIT_OUTPUT_SUBTYPE stageKitOutputSubtype = stringToStagekitOutputSubtype(output.getSubtype());
    switch (stageKitOutputSubtype) {
        case STAGEKIT_OUTPUT_SUBTYPE::Handle_LEDUpdate:
            return stringToSkRumbleData(output.getParameter("value"));

        case STAGEKIT_OUTPUT_SUBTYPE::Handle_StrobeUpdate:
            strobeSpeed = stringToInt(output.getParameter("value"));
            if ((0 > strobeSpeed) || (strobeSpeed > 4)) {
                throw std::out_of_range("Attempted to use Handle_StrobeUpdate value out of range.");
            }
            if (strobeSpeed == 0) {
                return SKRUMBLEDATA::SK_STROBE_OFF;
            } else {
                return stringToSkRumbleData("SK_STROBE_SPEED_" + std::to_string(strobeSpeed));
            }

        case STAGEKIT_OUTPUT_SUBTYPE::Handle_FogUpdate:
            fogOn = stringToBool(output.getParameter("value"));
            if (fogOn) {
                return SKRUMBLEDATA::SK_FOG_ON;
            } else {
                return SKRUMBLEDATA::SK_FOG_OFF;
            }

        default:
            throw std::invalid_argument("Invalid stageKitOutput subtype.");
    }
}
