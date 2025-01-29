#include "EventEngine.h"

// Constructor
EventEngine::EventEngine(const MappingConfig& mappings, RpiLightsController& rpiLightsController)
    : mappings(mappings), skProcessor(rpiLightsController), qlcProcessor(nullptr) {
    // You can initialize processors here
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
void EventEngine::handleInputEvent(const InputEvent& inputEvent) {
	const std::string inputId = inputEvent.getId();
    Input input = mappings.getInputById(inputId);

    MSG_EVENTENGINE_DEBUG( "handleInputEvent called for inputId " << inputId << "." );

    // Ignore SK_FOG_OFF spam
    if ((inputId == skRumbleDataTypeToString(SKRUMBLEDATA::SK_FOG_OFF)) &&
    	(inputId == lastInputId)) {
        MSG_EVENTENGINE_DEBUG( "Ignoring SK_FOG_OFF spam." );
    	return;
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
                MSG_EVENTENGINE_DEBUG( "Determined QlcplusProcessor should process." );
				qlcProcessor->process(outputToProcess);
				break;
			default:
				std::cerr << "Unknown output type!" << std::endl;
				break;
		}
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

    // Only doing this to prevent processing of SK_FOG_OFF spam.
    lastInputId = inputId;
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

    // If we found a match, erase it
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
    int strobeSpeed = 0;  // Default initialization
    bool fogOn = false;   // Default initialization

	STAGEKIT_OUTPUT_SUBTYPE stageKitOutputSubtype = stringToStagekitOutputSubtype(output.getSubtype());
	switch(stageKitOutputSubtype) {

		case STAGEKIT_OUTPUT_SUBTYPE::Handle_LEDUpdate:
			return stringToSkRumbleData(output.getParameter("value"));

		case STAGEKIT_OUTPUT_SUBTYPE::Handle_StrobeUpdate:
			strobeSpeed = stringToInt(output.getParameter("value"));
			if ((0 > strobeSpeed) || (strobeSpeed > 4)) {
		        throw std::out_of_range("Attempted to use Handle_StrobeUpdate value from MappingConfig: " + std::to_string(strobeSpeed) + ".  This value should be between 0 and 4.");
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
			throw std::invalid_argument("Attempted to use invalid stageKitOutput " + output.getType() + ":" + output.getSubtype() + ".");
			return SKRUMBLEDATA::SK_NONE;
	}

}
