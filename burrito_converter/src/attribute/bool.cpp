#include "bool.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// xml_attribute_to_bool
//
// Parses a bool from the value of a rapidxml::xmlattribute. "true" or "1" are
// evaluated as `true`. 'false' or '0' are evaluated as `false`. Everything
// else appends an error to the errors vector.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Bool::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    bool* value,
    bool* is_set
) {
    if (get_attribute_value(input) == "0" || get_attribute_value(input) == "false") {
        *value = false;
        *is_set = true;
    }
    else if (get_attribute_value(input) == "1" || get_attribute_value(input) == "true") {
        *value = true;
        *is_set = true;
    }
    else {
        errors->push_back(new XMLAttributeValueError("Found a boolean value that was not a '1', '0', 'true', or 'false'", input));
    }
}

////////////////////////////////////////////////////////////////////////////////
// bool_to_xml_attribute
//
// Converts a bool into a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Bool::to_xml_attribute(
    XMLWriterState*,
    const bool* value,
    std::function<void(std::string)> setter
) {
    if (*value) {
        setter("true");
    }
    else {
        setter("false");
    }
}

////////////////////////////////////////////////////////////////////////////////
// inverted_xml_attribute_to_bool
//
// Parses an inverted bool from the value of a rapidxml::xmlattribute. "true"
// or "1" are evaluated as `false`. 'false' or '0' are evaluated as `true`.
// Everything else appends an error to the errors vector.
////////////////////////////////////////////////////////////////////////////////
void Attribute::InvertBool::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    bool* value,
    bool* is_set
) {
    if (get_attribute_value(input) == "0" || get_attribute_value(input) == "false") {
        *value = true;
        *is_set = true;
    }
    else if (get_attribute_value(input) == "1" || get_attribute_value(input) == "true") {
        *value = false;
        *is_set = true;
    }
    else {
        errors->push_back(new XMLAttributeValueError("Found a boolean value that was not a '1', '0', 'true', or 'false'", input));
    }
}

////////////////////////////////////////////////////////////////////////////////
// bool_to_inverted_xml_attribute
//
// Inverts and converts a bool into a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
void Attribute::InvertBool::to_xml_attribute(
    XMLWriterState*,
    const bool* value,
    std::function<void(std::string)> setter
) {
    if (*value) {
        setter("false");
    }
    else {
        setter("true");
    }
}

////////////////////////////////////////////////////////////////////////////////
// proto_to_bool
//
// Parses a bool from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Bool::from_proto_field(
    bool input,
    ProtoReaderState*,
    bool* value,
    bool* is_set
) {
    *value = input;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// bool_to_proto
//
// Writes a bool to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Bool::to_proto_field(
    bool value,
    ProtoWriterState*,
    std::function<void(bool)> setter
) {
    setter(value);
}
