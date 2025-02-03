#include "MappingConfig.h"

void MappingConfig::addMapping(const Mapping& mapping) {
    mappings.push_back(mapping);
}

const std::vector<Mapping>& MappingConfig::getMappings() const {
    return mappings;
}

const Input& MappingConfig::getInputById(const std::string& id) const {
    for (auto& mapping : mappings) {
        if (mapping.getInput().getId() == id) {
            return mapping.getInput(); // Return the matching Input object
        }
    }
    throw std::runtime_error("Input with id '" + id + "' not found in MappingConfig.");
}

const std::vector<Output>& MappingConfig::getOutputsByInputId(const std::string& id) const {
	for (auto& mapping : mappings) {
		if (mapping.getInput().getId() == id) {
			return mapping.getOutputs(); // Return the Outputs for the matching inputId
		}
	}
	throw std::runtime_error("Input with id '" + id + "' not found in MappingConfig.");
}

const std::vector<Input> MappingConfig::getFileExistsInputs() const {
	std::vector<Input> fileExistsInputs;
	for (auto& mapping : mappings) {
		Input input = mapping.getInput();
		if (input.getType() == "fileExistsInput") {
			fileExistsInputs.push_back(input);
		}
	}
	return fileExistsInputs;
}
