#include "color.hpp"

#include <stdint.h>

#include <iosfwd>
#include <sstream>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// parse_color
//
// Parses a Color from the value of a rapidxml::xml_attribute.
// TODO(#98): Color should be saved in a better format then the raw hex string.
// TODO(#129): Compound Value XML Export
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
waypoint::RGBAColor* to_proto_color(Color attribute_value) {
    string hex = attribute_value.hex;
    waypoint::RGBAColor* color = new waypoint::RGBAColor;
    // Adding default values until TODO #98
    int r = 255;
    int g = 255;
    int b = 255;
    int a = 255;
    uint32_t rgba = ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);
    color->set_rgba_color(rgba);
    return color;
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_color
//
// Converts a proto message into a Color
////////////////////////////////////////////////////////////////////////////////
Color from_proto_color(waypoint::RGBAColor attribute_value) {
    Color color;
    std::stringstream stream;
    stream << std::hex << attribute_value.rgba_color();
    std::string rgba = stream.str();

    color.hex = rgba.substr(0, 6);
    // Adding default values until TODO #98
    color.alpha = 1.0;
    return color;
}
