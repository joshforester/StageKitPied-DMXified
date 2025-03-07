#ifndef MAPPING_H
#define MAPPING_H

#include "Input.h"
#include "Output.h"
#include "OutputOverride.h"
#include <vector>

class Mapping {
public:
    Mapping(rapidxml::xml_node<>* node);

    // Getters
    const Input& getInput() const;
    const std::vector<Output>& getOutputs() const;

private:
    Input input;
    std::vector<Output> outputs;
};

#endif // MAPPING_H
