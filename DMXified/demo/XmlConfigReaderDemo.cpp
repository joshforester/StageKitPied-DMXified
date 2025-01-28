#include <iostream>
#include "../config/XmlLoader.h"
#include "../config/MappingConfig.h"

int main(int argc, char* argv[]) {

    // Default value
    std::string inputFile = "demo_mapping.xml";

    // Check if a command-line argument is provided
    if (argc == 2) {
        // If provided, use the first argument as the input file name
        inputFile = argv[1];
    } else if (argc > 2) {
    	std::cerr << "Too many arguments provided.\n";
    	std::cout << "Usage: " << argv[0] << " [mappingFile]\n";

    	return -1;
    }

    // Print the file name
    std::cout << "Input file: " << inputFile << std::endl;

    try {
        // Load the XML file into a MappingConfig object
        MappingConfig config = XmlLoader::loadMappingConfig(inputFile);

        // Iterate through the Mappings in the config and print their details
        for (Mapping& mapping : config.getMappings()) {
            std::cout << "Input: " << mapping.getInput().getType() << ":" << mapping.getInput().getId() << "(P:" << mapping.getInput().getPriority() << ")\n";
            if (mapping.getInput().getType() == "fileExistsInput") {
            	std::cout << "File: " << mapping.getInput().getFile() << "\n";
            }
            std::cout << "Output Override List:\n";
            for (const auto& outputOverride : mapping.getInput().getOutputOverrideList()) {
                std::cout << "  " << outputOverride.getType() << ":" << outputOverride.getSubtype() << "\n";
            }

            std::cout << "Outputs:\n";
            for (const auto& output : mapping.getOutputs()) {
                std::cout << "  Output: " << output.getType() << ":" << output.getSubtype() << "\n";
                std::cout << "  Parameters:\n";
                for (const auto& key : output.getParameterTypes()) {
                  std::cout << "    " << key << ":" << output.getParameter(key) << "\n";
                }
            }
            std::cout << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
