#include "float.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads a float from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Float::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>*,
    XMLReaderState*,
    float* value,
    bool* is_set
) {
    *value = std::stof(get_attribute_value(input));
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Writes a float to an xml attribute using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Float::to_xml_attribute(
    XMLWriterState*,
    const float* value,
    std::function<void(std::string)> setter
) {
    setter(to_string(*value));
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads a float from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Float::from_proto_field(
    float input,
    ProtoReaderState*,
    float* value,
    bool* is_set
) {
    *value = input;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes a float to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Float::to_proto_field(
    float value,
    ProtoWriterState*,
    std::function<void(float&)> setter
) {
    setter(value);
}
