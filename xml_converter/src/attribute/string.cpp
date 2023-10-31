#include "string.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// parse_string
//
// Parses a string from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
string parse_string(rapidxml::xml_attribute<>* input, vector<XMLError*>*) {
    return get_attribute_value(input);
}

////////////////////////////////////////////////////////////////////////////////
// string_to_xml_attribute
//
// Converts a string into a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
string string_to_xml_attribute(const string& attribute_name, const string* value) {
    return " " + attribute_name + "=\"" + *value + "\"";
}
