#include "bool.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads a bool from an xml_attribute. "true" and "1" are evaluated as `true`.
// 'false' and '0' are evaluated as `false`. Everything else will trigger an
// error to be added to the errors vector.
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
// to_xml_attribute
//
// Writes a bool to an xml attribute using the provided setter function.
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
// from_proto_field
//
// Reads a bool from a proto field.
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
// to_proto_field
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

////////////////////////////////////////////////////////////////////////////////
// InvertBool::from_xml_attribute
//
// Reads a bool from an xml attribute and inverts it. The logic for reading
// the xml attribute is the inverse of Attribute::Bool::from_xml_attribute().
////////////////////////////////////////////////////////////////////////////////
void Attribute::InvertBool::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* reader_state,
    bool* value,
    bool* is_set
) {
    Attribute::Bool::from_xml_attribute(
        input,
        errors,
        reader_state,
        value,
        is_set
    );

    *value = !*value;
}

////////////////////////////////////////////////////////////////////////////////
// InvertBool::to_xml_attribute
//
// Inverts and writes a bool to to an xml attribute using the provided
// setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::InvertBool::to_xml_attribute(
    XMLWriterState* xml_writer_state,
    const bool* value,
    std::function<void(std::string)> setter
) {
    bool inverted_value = !*value;
    Attribute::Bool::to_xml_attribute(
        xml_writer_state,
        &inverted_value,
        setter
    );
}
