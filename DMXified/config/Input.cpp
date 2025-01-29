#include "Input.h"

Input::Input(rapidxml::xml_node<>* node) {
    // Parse attributes
    type = node->first_attribute("type")->value();
    id = node->first_attribute("id")->value();
    priority = std::stoi(node->first_attribute("priority")->value());

    // Parse optional <file> element
    rapidxml::xml_node<>* fileNode = node->first_node("file");
    if (fileNode) {
        file = fileNode->value();
    }

    // Parse optional <outputOverrideList> node
    rapidxml::xml_node<>* overrideListNode = node->first_node("outputOverrideList");
    if (overrideListNode) {
        for (rapidxml::xml_node<>* overrideNode = node->first_node("outputOverrideList")->first_node("output"); overrideNode; overrideNode = overrideNode->next_sibling()) {
            outputOverrideList.push_back(OutputOverride(overrideNode));
        }
    }
}

std::string Input::getType() const { return type; }
std::string Input::getId() const { return id; }
int Input::getPriority() const { return priority; }

std::string Input::getFile() const {
	return file;
}

bool Input::hasFile() const { return !file.empty(); }

bool Input::isNegationEvent() const {
    // Check if the string has at least 3 characters
    if (id.size() < 3) {
        return false;
    }

    // Check the last 3 characters
    return id.substr(id.size() - 3) == "Off";
}

std::string Input::getNegatedInputId() const {
	if (this->isNegationEvent()) {
		return id.substr(0, id.length() - 3);
	}

	throw std::runtime_error("Attempting to getNegatedInputId when " + id + " is not a negation event.");
}

std::vector<OutputOverride> Input::getOutputOverrideList() const {
    return outputOverrideList;
}

void Input::populateFromNegatedInput(const Input& negatedInput) {
    file = negatedInput.getFile();
    outputOverrideList = negatedInput.getOutputOverrideList();
}
