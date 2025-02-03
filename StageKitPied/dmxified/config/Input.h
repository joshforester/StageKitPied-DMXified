#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <vector>
#include <stdexcept>
#include <rapidxml.hpp>
#include "OutputOverride.h"

// Represents the <input> element
class Input {
public:
    Input(rapidxml::xml_node<>* node);

    // Getters
    std::string getType() const;
    std::string getId() const;
    int getPriority() const;
    std::string getFile() const;                    // Optional file attribute
    bool hasFile() const;                           // Check if file exists
    bool isNegationEvent() const;                   // Check if this event negates another
    std::string getNegatedInputId() const;          // Get the id of the input this event negates
    // OutputOverrideList getter
    std::vector<OutputOverride> getOutputOverrideList() const;

    // Copy the file and outputOverrideList from the negatedInput to this one.
    void populateFromNegatedInput(const Input& negatedInput);

private:
    std::string type;
    std::string id;
    int priority;
    std::string file;                               // Optional file attribute for <fileExistsInput>
    std::vector<OutputOverride> outputOverrideList; // List of output overrides
};

#endif // INPUT_H

