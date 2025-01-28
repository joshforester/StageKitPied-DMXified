#include "Output.h"
#include <stdexcept>

Output::Output(rapidxml::xml_node<>* node) {
    type = node->first_attribute("type")->value();
    subtype = node->first_attribute("subtype")->value();

    // Parse parameters (sub-elements like <widgetId>, <value>, etc.)
    for (rapidxml::xml_node<>* paramNode = node->first_node(); paramNode; paramNode = paramNode->next_sibling()) {
        parameters.push_back({paramNode->name(), paramNode->value()});
    }
}

std::string Output::getType() const { return type; }
std::string Output::getSubtype() const { return subtype; }

std::vector<std::string> Output::getParameterTypes() const {
	std::vector<std::string> keys;
	for (const auto& param : parameters) {
		keys.push_back(param.first);
	}
	return keys;
}
std::string Output::getParameter(const std::string& name) const {
    for (const auto& param : parameters) {
        if (param.first == name) {
            return param.second;
        }
    }
    throw std::invalid_argument("Parameter not found");
}
