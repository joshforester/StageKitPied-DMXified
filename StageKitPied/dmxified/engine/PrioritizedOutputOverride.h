#ifndef PRIORITIZEDOUTPUTOVERRIDE_H
#define PRIORITIZEDOUTPUTOVERRIDE_H

#include <string>

class PrioritizedOutputOverride {
public:
    // Constructor
    PrioritizedOutputOverride(const std::string& type, const std::string& subtype, int priority);

    // Getters
    const std::string& getType() const;
    const std::string& getSubtype() const;
    int getPriority() const;

    // Equality operator for comparison
    bool operator==(const PrioritizedOutputOverride& other) const;

private:
    std::string type;
    std::string subtype;
    int priority;
};

#endif // PRIORITIZEDOUTPUTOVERRIDE_H
