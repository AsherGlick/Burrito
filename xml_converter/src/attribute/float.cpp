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
void xml_attribute_to_float(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    float* value,
    bool* is_set) {
    *value = std::stof(get_attribute_value(input));
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// float_to_xml_attribute
//
// Converts a float into a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
string float_to_xml_attribute(const string& attribute_name, const float* value) {
    return " " + attribute_name + "=\"" + to_string(*value) + "\"";
}

////////////////////////////////////////////////////////////////////////////////
// proto_to_float
//
// Parses a float from a proto field.
////////////////////////////////////////////////////////////////////////////////
void proto_to_float(float input, float* value, bool* is_set) {
    *value = input;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// float_to_proto
//
// Writes a float to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void float_to_proto(float value, std::function<void(float&)> setter) {
    setter(value);
}

////////////////////////////////////////////////////////////////////////////////
// replace_one_point_zero_with_zero
//
// Parses a float from the value of a rapidxml::xml_attribute and
// replaces default value of one with zero
////////////////////////////////////////////////////////////////////////////////
void default_value_one_xml_attribute_to_float(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    float* value,
    bool* is_set) {
    if (std::stof(get_attribute_value(input)) == 1.0) {
        *value = 0;
    }
    else {
        *value = std::stof(get_attribute_value(input));
        *is_set = true;
    }
}
