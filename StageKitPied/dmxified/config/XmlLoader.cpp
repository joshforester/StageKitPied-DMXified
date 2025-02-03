#include "XmlLoader.h"


MappingConfig XmlLoader::loadMappingConfig(const std::string& filename) {
    // Read the XML file into a string
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open XML file");
    }

    std::string xmlContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Parse the XML content using rapidxml
    rapidxml::xml_document<> doc;
    doc.parse<0>(&xmlContent[0]);

    // Create and populate MappingConfig
    MappingConfig config;

    rapidxml::xml_node<>* rootNode = doc.first_node("root");
    for (rapidxml::xml_node<>* mappingNode = rootNode->first_node("mapping"); mappingNode; mappingNode = mappingNode->next_sibling()) {
        Mapping mapping(mappingNode);
        config.addMapping(mapping);
    }

    return config;
}
