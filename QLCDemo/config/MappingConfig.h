#ifndef MAPPINGCONFIG_H
#define MAPPINGCONFIG_H

#include <vector>
#include "Mapping.h"

class MappingConfig {
public:
    // Adds a Mapping object to the configuration
    void addMapping(const Mapping& mapping);

    // Gets the list of Mappings in the config
    std::vector<Mapping>& getMappings();

private:
    std::vector<Mapping> mappings;  // Holds all Mapping objects
};

#endif // MAPPINGCONFIG_H
