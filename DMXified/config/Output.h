#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>
#include <vector>
#include <rapidxml.hpp>

// Represents the <output> element
class Output {
public:
    Output(rapidxml::xml_node<>* node);

    // Getters
    std::string getType() const;
    std::string getSubtype() const;
    std::vector<std::string> getParameterTypes() const;
    std::string getParameter(const std::string& name) const;

private:
    std::string type;
    std::string subtype;
    std::vector<std::pair<std::string, std::string>> parameters; // Name-value pairs for sub-elements like <widgetId>, <value>, etc.
};

#endif // OUTPUT_H
