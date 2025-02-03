#include "PrioritizedOutputOverride.h"

// Constructor definition
PrioritizedOutputOverride::PrioritizedOutputOverride(const std::string& type, const std::string& subtype, int priority)
    : type(type), subtype(subtype), priority(priority) {}

// Getter definitions
const std::string& PrioritizedOutputOverride::getType() const {
    return type;
}

const std::string& PrioritizedOutputOverride::getSubtype() const {
    return subtype;
}

int PrioritizedOutputOverride::getPriority() const {
    return priority;
}

// Equality operator definition
bool PrioritizedOutputOverride::operator==(const PrioritizedOutputOverride& other) const {
    return type == other.type && subtype == other.subtype;
}
