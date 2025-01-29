#ifndef CONFIG_CONFIGHELPERS_H_
#define CONFIG_CONFIGHELPERS_H_


#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>  // For std::transform

inline bool stringToBool(const std::string& str) {
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);

    if (lower_str == "true") {
        return true;
    } else if (lower_str == "false") {
        return false;
    } else {
        throw std::invalid_argument("Attempted to convert invalid string value for boolean from MappingConfig: " + str + ".  This is likely a Handle_FogUpdate value.");
    }
}

inline int stringToInt(const std::string& str) {
    try {
        return std::stoi(str);  // This will throw exceptions if invalid
    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Attempted to convert invalid string value for integer from MappingConfig: " + str + ".  This is likely a Handle_StrobeUpdate value.");
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Attempted to convert invalid string value for integer (out of range) from MappingConfig: " + str + ".  This is likely a Handle_StrobeUpdate value.");
    }
}


#endif /* CONFIG_CONFIGHELPERS_H_ */
