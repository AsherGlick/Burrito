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
// stringify_string
//
// Returns the same string that was passed in which is encoded directly into
// xml. This function exists for stylistic convenience with all the other
// attribute stringify functions.
////////////////////////////////////////////////////////////////////////////////
string stringify_string(string attribute_value) {
    return attribute_value;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_string
//
// Returns the same string that was passed in which is encoded directly into
// proto. This function exists for stylistic convenience with all the other
// attribute stringify functions.
////////////////////////////////////////////////////////////////////////////////
string to_proto_string(string attribute_value) {
    return attribute_value;
}
