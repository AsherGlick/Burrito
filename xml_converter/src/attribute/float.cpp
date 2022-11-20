#include "float.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

////////////////////////////////////////////////////////////////////////////////
// parse_float
//
// Parses a float from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
float parse_float(rapidxml::xml_attribute<>* input, std::vector<XMLError*>*) {
    return std::stof(get_attribute_value(input));
}

////////////////////////////////////////////////////////////////////////////////
// stringify_float
//
// Converts a float into a stringy value so that it can be saved to xml.
////////////////////////////////////////////////////////////////////////////////
std::string stringify_float(float attribute_value) {
    return std::to_string(attribute_value);
}
