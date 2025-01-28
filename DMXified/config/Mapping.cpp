#include "Mapping.h"

Mapping::Mapping(rapidxml::xml_node<>* node)
	: input(node->first_node("input"))
{
    // Parse <outputs> nodes
    for (rapidxml::xml_node<>* outputNode = node->first_node("outputs")->first_node("output"); outputNode; outputNode = outputNode->next_sibling()) {
        outputs.push_back(Output(outputNode));
    }
}

Input& Mapping::getInput() { return input; }
std::vector<Output>& Mapping::getOutputs() { return outputs; }
