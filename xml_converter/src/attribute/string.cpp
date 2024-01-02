#include "string.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// xml_attribute_to_string
//
// Parses a string from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
void xml_attribute_to_string(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    string* value,
    bool* is_set) {
    *value = get_attribute_value(input);
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// string_to_xml_attribute
//
// Converts a string into a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
string string_to_xml_attribute(
    const string& attribute_name,
    XMLWriterState* state,
    const string* value) {
    return " " + attribute_name + "=\"" + *value + "\"";
}

////////////////////////////////////////////////////////////////////////////////
// proto_to_string
//
// Parses a string from a proto field.
////////////////////////////////////////////////////////////////////////////////
void proto_to_string(
    string input,
    ProtoReaderState* state,
    string* value,
    bool* is_set) {
    *value = input;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// string_to_proto
//
// Writes a string to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void string_to_proto(
    std::string value,
    ProtoWriterState* state,
    std::function<void(std::string)> setter) {
    setter(value);
}

void proto_display_name_to_display_name_and_name(
    std::string input,
    ProtoReaderState* state,
    std::string* display_name,
    bool* is_display_name_set,
    std::string* name,
    bool* is_name_set) {
    *display_name = input;
    *is_display_name_set = true;
    *name = normalize(input);
    *is_name_set = true;
}

void display_name_and_name_to_proto_display_name(
    std::string value,
    ProtoWriterState* state,
    std::function<void(std::string)> setter,
    const std::string* name,
    const bool* is_name_set) {
    setter(value);
}
