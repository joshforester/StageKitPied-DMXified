#include "OutputOverride.h"

OutputOverride::OutputOverride(rapidxml::xml_node<>* node) {
    type = node->first_attribute("type")->value();
    subtype = node->first_attribute("subtype")->value();
}

std::string OutputOverride::getType() const { return type; }
std::string OutputOverride::getSubtype() const { return subtype; }
