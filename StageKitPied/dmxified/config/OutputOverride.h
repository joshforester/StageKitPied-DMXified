#ifndef OUTPUTOVERRIDE_H
#define OUTPUTOVERRIDE_H

#include <rapidxml.hpp>
#include <string>

// Represents the <outputOverride> element inside <outputOverrideList>
class OutputOverride {
public:
    OutputOverride(rapidxml::xml_node<>* node);

    // Getters
    std::string getType() const;
    std::string getSubtype() const;

private:
    std::string type;
    std::string subtype;
};

#endif // OUTPUTOVERRIDE_H
