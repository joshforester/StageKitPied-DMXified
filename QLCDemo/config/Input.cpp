#include "Input.h"
#include <stdexcept>

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
    if (hasFile()) {
        return file;
    }
    throw std::runtime_error("File does not exist for this Input.");
}

bool Input::hasFile() const { return !file.empty(); }

std::vector<OutputOverride> Input::getOutputOverrideList() const {
    return outputOverrideList;
}

