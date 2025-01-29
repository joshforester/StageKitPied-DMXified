#include <iostream>
#include "../config/XmlLoader.h"
#include "../config/MappingConfig.h"

int main(int argc, char* argv[]) {

    // Default value
    std::string mappingFileName = "demo_mapping.xml";

    // Check if a command-line argument is provided
    if (argc == 2) {
        // If provided, use the first argument as the mapping file name
        mappingFileName = argv[1];
    } else if (argc > 2) {
    	std::cerr << "Too many arguments provided.\n";
    	std::cout << "Usage: " << argv[0] << " [mappingFile]\n";
    	return -1;
    }

    // Print the file name
    std::cout << "Mapping file: " << mappingFileName << std::endl;

    try {
        // Load the XML file into a MappingConfig object
        MappingConfig config = XmlLoader::loadMappingConfig(mappingFileName);

        // Iterate through the Mappings in the config and print their details
        for (const Mapping& mapping : config.getMappings()) {
        	Input input = mapping.getInput();

            std::cout << "Input: " << input.getType() << ":" << input.getId() << "(P:" << input.getPriority() << ")";
            const bool isNegationEvent = input.isNegationEvent();
            if (isNegationEvent) {
            	std::cout << " [Negating event for " << input.getNegatedInputId() << "]";
            }
            std::cout << std::endl;

            if (!isNegationEvent) {
				if (input.getType() == "fileExistsInput") {
					std::cout << "File: " << input.getFile() << std::endl;
				}
				std::cout << "Output Override List:" << std::endl;
				for (const auto& outputOverride : input.getOutputOverrideList()) {
					std::cout << "  " << outputOverride.getType() << ":" << outputOverride.getSubtype() << std::endl;
				}
            }

            std::cout << "Outputs:" << std::endl;
            for (const auto& output : mapping.getOutputs()) {
                std::cout << "  Output: " << output.getType() << ":" << output.getSubtype() << std::endl;
                std::cout << "  Parameters:" << std::endl;
                for (const auto& key : output.getParameterTypes()) {
                  std::cout << "    " << key << ":" << output.getParameter(key) << std::endl;
                }
            }
            std::cout << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
