#ifndef MAPPINGCONFIG_H
#define MAPPINGCONFIG_H

#include <vector>
#include <stdexcept>
#include "Mapping.h"

class MappingConfig {
public:
    // Adds a Mapping object to the configuration
    void addMapping(const Mapping& mapping);

    // Gets the list of Mappings in the config
    const std::vector<Mapping>& getMappings() const;

    // Convenience method to get an Input based on the inputId
    const Input& getInputById(const std::string& id) const;

    // Convenience method to get lists of outputs based on the inputId
    const std::vector<Output>& getOutputsByInputId(const std::string& id) const;

private:
    std::vector<Mapping> mappings;  // Holds all Mapping objects
};

#endif // MAPPINGCONFIG_H
