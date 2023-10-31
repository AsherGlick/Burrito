#include "float.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;
////////////////////////////////////////////////////////////////////////////////
// parse_float
//
// Parses a float from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
float parse_float(rapidxml::xml_attribute<>* input, std::vector<XMLError*>*) {
    return std::stof(get_attribute_value(input));
}

////////////////////////////////////////////////////////////////////////////////
// float_to_xml_attribute
//
// Converts a float into a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
string float_to_xml_attribute(const string& attribute_name, const float* value) {
    return " " + attribute_name + "=\"" + to_string(*value) + "\"";
}
