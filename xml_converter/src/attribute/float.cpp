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
    std::vector<XMLError*>*,
    XMLReaderState*,
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
string float_to_xml_attribute(
    const string& attribute_name,
    XMLWriterState*,
    const float* value) {
    return " " + attribute_name + "=\"" + to_string(*value) + "\"";
}

////////////////////////////////////////////////////////////////////////////////
// proto_to_float
//
// Parses a float from a proto field.
////////////////////////////////////////////////////////////////////////////////
void proto_to_float(
    float input,
    ProtoReaderState*,
    float* value,
    bool* is_set) {
    *value = input;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// float_to_proto
//
// Writes a float to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void float_to_proto(
    float value,
    ProtoWriterState*,
    std::function<void(float&)> setter) {
    setter(value);
}
