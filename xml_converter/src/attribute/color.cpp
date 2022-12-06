#include "color.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// parse_color
//
// Parses a Color from the value of a rapidxml::xml_attribute.
// TODO(#98): Color should be saved in a better format then the raw hex string.
////////////////////////////////////////////////////////////////////////////////
Color parse_color(rapidxml::xml_attribute<>* input, vector<XMLError*>*) {
    Color color;
    color.hex = get_attribute_value(input);
    return color;
}

////////////////////////////////////////////////////////////////////////////////
// stringify_color
//
// Converts a Color into a stringy value so it can be saved to xml.
////////////////////////////////////////////////////////////////////////////////
string stringify_color(Color attribute_value) {
    return attribute_value.hex;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_color
//
// Converts a Color into a proto message
////////////////////////////////////////////////////////////////////////////////
waypoint::Color* to_proto_color(Color attribute_value) {
    waypoint::Color* color = new waypoint::Color();
    color->set_hex(attribute_value.hex);
    return color;
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_color
//
// Converts a proto message into a Color
////////////////////////////////////////////////////////////////////////////////
Color from_proto_color(waypoint::Color attribute_value) {
    Color color;
    color.hex = attribute_value.hex();
    return color;
}