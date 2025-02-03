#include "StageKitOutputProcessor.h"

StageKitOutputProcessor::StageKitOutputProcessor(RpiLightsController& rpiLightsController)
    : rpiLightsController(rpiLightsController) {} // Initialize the reference to RpiLightsController

// The process function that processes the output based on weights
void StageKitOutputProcessor::process(Output output, uint8_t left_weight, uint8_t right_weight) const {
//    std::lock_guard<std::mutex> lock(mtx);  // Locking before calling methods on shared resource

	std::string message = "process called for output " + output.getType() + ":" + output.getSubtype() + ".";
	MSG_STAGEKITOUTPUTPROCESSOR_DEBUG( message );

    // Trigger the event using RpiLightsController
    rpiLightsController.Do_Handle_RumbleData(left_weight, right_weight);
}
