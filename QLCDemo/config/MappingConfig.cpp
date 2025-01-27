#include "MappingConfig.h"

void MappingConfig::addMapping(const Mapping& mapping) {
    mappings.push_back(mapping);
}

std::vector<Mapping>& MappingConfig::getMappings() {
    return mappings;
}
