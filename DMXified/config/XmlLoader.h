#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <rapidxml.hpp>
#include "MappingConfig.h"

class XmlLoader {
public:
    // Loads the XML from a file and parses it into a MappingConfig object
    static MappingConfig loadMappingConfig(const std::string& filename);

private:
    // Private constructor to prevent instantiation
    XmlLoader() {}
};

#endif // XMLLOADER_H
