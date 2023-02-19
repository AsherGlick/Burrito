#include "color.hpp"

#include <stdint.h>

#include <cmath>
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
// TODO: Every node that has a Hex needs an alpha but not every alpha needs Hex
////////////////////////////////////////////////////////////////////////////////
Color parse_color(rapidxml::xml_attribute<>* input, vector<XMLError*>*) {
    Color color;
    std::string hex = get_attribute_value(input);
    if (hex.length() == 6 || 7) {
        if (hex.compare(0, 1, "#")) {
            hex = hex.erase(0, 1);
        }
        color.hex = hex;
    }
    return color;
}

////////////////////////////////////////////////////////////////////////////////
// stringify_color
//
// Converts a Color into a stringy value so it can be saved to xml.
////////////////////////////////////////////////////////////////////////////////
string stringify_color(Color attribute_value) {
    return "#" + attribute_value.hex;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_color
//
// Converts a Color into a proto message
// TODO: Find a way to transfer alpha when color hex is not set
////////////////////////////////////////////////////////////////////////////////
waypoint::RGBA* to_proto_color(Color attribute_value, float alpha) {
    waypoint::RGBA* rgba = new waypoint::RGBA();
    string hex = attribute_value.hex;
    int r = stoi(hex.substr(0, 2), 0, 16);
    int g = stoi(hex.substr(2, 2), 0, 16);
    int b = stoi(hex.substr(4, 2), 0, 16);

    int a;
    if (alpha == 1.0) {
        a = 255;
    }
    else {
        a = (int)(alpha * 256);
    }

    uint32_t rgba_int = ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);

    rgba->set_rgba(rgba_int);
    return rgba;
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_color
//
// Converts a proto message into a Color
////////////////////////////////////////////////////////////////////////////////
Color from_proto_color(waypoint::RGBA attribute_value) {
    Color color;
    std::stringstream stream;
    stream << std::hex << attribute_value.rgba();
    std::string rgba = stream.str();

    color.hex = rgba.substr(0, 6);
    color.alpha = std::stof(rgba.substr(6, 2));
    return color;
}
