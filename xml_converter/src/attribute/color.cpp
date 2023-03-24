#include "color.hpp"

#include <stdint.h>

#include <iosfwd>
#include <ostream>
#include <regex>

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
    std::string input_string = get_attribute_value(input);
    std::string hex_string;

    if (input_string.size() > 0 && input_string[0] == '#') {
        hex_string = input_string.substr(1);
    }
    else {
        hex_string = input_string;
    }

    std::regex hex_pattern("^([A-Fa-f0-9]{6})$|^([A-Fa-f0-9]{8})$");

    if (std::regex_match(hex_string, hex_pattern)) {
        // Extract the R, G, B, and A values from the Hex string
        if (hex_string.size() == 6 || hex_string.size() == 8) {
            color.r = std::stoi(hex_string.substr(0, 2), nullptr, 16);
            color.g = std::stoi(hex_string.substr(2, 2), nullptr, 16);
            color.b = std::stoi(hex_string.substr(4, 2), nullptr, 16);
            if (hex_string.size() == 8) {
                color.a = std::stoi(hex_string.substr(6, 2), nullptr, 16);
            }
            else {
                color.a = 255;
            }
        }
    }
    return color;
}

////////////////////////////////////////////////////////////////////////////////
// stringify_color
//
// Converts a Color into a stringy value so it can be saved to xml.
////////////////////////////////////////////////////////////////////////////////
string stringify_color(Color attribute_value) {
    std::stringstream stream;
    std::string hex_string = "#";

    stream << std::hex << attribute_value.r;
    hex_string += stream.str();

    stream << std::hex << attribute_value.g;
    hex_string += stream.str();

    stream << std::hex << attribute_value.b;
    hex_string += stream.str();

    std::string rgb = hex_string;
    return rgb;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_color
//
// Converts a Color into a proto message
////////////////////////////////////////////////////////////////////////////////
waypoint::RGBAColor* to_proto_color(Color attribute_value) {
    waypoint::RGBAColor* color = new waypoint::RGBAColor();
    // The default RGB in burrito will be 000000 (i.e. black)

    // If alpha (float) is not the default value, convert to int
    if (attribute_value.alpha != 0) {
        int alpha_int = static_cast<int>(attribute_value.alpha * 255);
        // Check that it doesnt exceed the bounds of {0,255}
        if (alpha_int > 255) {
            alpha_int = 255;
        }
        if (alpha_int < 0) {
            alpha_int = 0;
        }
        attribute_value.a = alpha_int;
    }
    else {
        // Default value of alpha in Burrito
        attribute_value.a = 255;
    }

    uint32_t rgba = ((attribute_value.r & 0xff) << 24) + ((attribute_value.g & 0xff) << 16) + ((attribute_value.b & 0xff) << 8) + (attribute_value.a & 0xff);
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
    std::string hex_string = stream.str();

    color.r = std::stoi(hex_string.substr(0, 2), nullptr, 16);
    color.g = std::stoi(hex_string.substr(2, 2), nullptr, 16);
    color.b = std::stoi(hex_string.substr(4, 2), nullptr, 16);
    color.a = std::stoi(hex_string.substr(6, 2), nullptr, 16);

    color.red = static_cast<float>(color.r) / 255.0f;
    color.green = static_cast<float>(color.g) / 255.0f;
    color.blue = static_cast<float>(color.b) / 255.0f;
    color.alpha = static_cast<float>(color.a) / 255.0f;

    return color;
}
