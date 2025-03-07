#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>  // For std::this_thread::sleep_for
#include <chrono>  // For std::chrono::seconds
#include "../engine/EventEngine.h"
#include "../engine/InputEvent.h"
#include "../config/XmlLoader.h"
#include "../config/MappingConfig.h"
#include "../../stagekit/StageKitConsts.h"

int main(int argc, char* argv[]) {

    // Default value
    std::string mappingFileName = "eesd_demo_mapping.xml";
    std::string eventFileName = "eesd_demo_events.txt";

    // Check if a command-line argument is provided
    if (argc == 3) {
        // If provided, use the first argument as the mapping file name and the second as the event file.
        mappingFileName = argv[1];
        eventFileName = argv[2];
    } else if (argc == 2) {
    	std::cerr << "Too few arguments provided.\n";
    	std::cout << "Usage: " << argv[0] << " [mappingFile] [eventFile]\n";
    	return -1;
    } else if (argc > 2) {
    	std::cerr << "Too many arguments provided.\n";
    	std::cout << "Usage: " << argv[0] << " [mappingFile] [eventFile]\n";
    	return -1;
    }

    // Print the file names
    std::cout << "Mapping file: " << mappingFileName << std::endl;
    std::cout << "Event file: " << eventFileName << std::endl;

    // Open even file and read lines.
    std::ifstream eventFile(eventFileName);
	if (!eventFile.is_open()) {
		std::cerr << "Error opening event file: " << eventFileName << std::endl;
		return -2;
	}
	std::vector<std::string> inputEventIds;
	std::string inputEventId;
	while (std::getline(eventFile, inputEventId)) {
		inputEventIds.push_back(inputEventId);
	}
	eventFile.close();

    // Load the XML file into a MappingConfig object
	MappingConfig config;
    try {
        config = XmlLoader::loadMappingConfig(mappingFileName);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return -3;
    }

    // Initialize the EventEngine
    RpiLightsController rpiLightsController("mock.INI");

    // Access the singleton instance of EventEngine
    EventEngine& eventEngine = EventEngine::getInstance(config, rpiLightsController);

    // Feed the EventEngine events, have it log its decisions with sleep.
    bool isFirst = true;
    const std::string inputDelimiter = "------------------------------";
    for (const auto& inputEventId : inputEventIds) {

    	if (isFirst) {
            std::cout << inputDelimiter << std::endl;
            isFirst = false;
    	}

    	InputEvent inputEventObj;

    	// Since we are simulating, we'll send partial data with inputEvent
    	if (config.getInputById(inputEventId).getType() == "stageKitInput") {
    		inputEventObj = InputEvent(inputEventId, SKRUMBLEDATA::SK_NONE, stringToSkRumbleData(inputEventId));
    	} else {
    		inputEventObj = InputEvent(inputEventId);
    	}

        // Handle the event
        eventEngine.handleInputEvent(inputEventObj);

        std::cout << inputDelimiter << std::endl;
    }

    return 0;
}
