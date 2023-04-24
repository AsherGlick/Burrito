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
// int_to_float // float_to_int
//
// Helper functions to convert the value of colors from one type to another.
// Also serves to make sure the values stay within the bounds.
////////////////////////////////////////////////////////////////////////////////
float int_to_float(int input) {
    if (input > 255) {
        input = 255;
    }
    if (input < 0) {
        input = 0;
    }
    float output = static_cast<float>(input) / 255.0f;
    return output;
}

int float_to_int(float input) {
    if (input > 1.0) {
        input = 1.0;
    }
    if (input < 0) {
        input = 0;
    }
    int output = static_cast<int>(input * 255);
    return output;
}

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
            color.red = int_to_float(std::stoi(hex_string.substr(0, 2), nullptr, 16));
            color.green = int_to_float(std::stoi(hex_string.substr(2, 2), nullptr, 16));
            color.blue = int_to_float(std::stoi(hex_string.substr(4, 2), nullptr, 16));
            if (hex_string.size() == 8) {
                color.alpha = int_to_float(std::stoi(hex_string.substr(6, 2), nullptr, 16));
            }
            else {
                color.alpha = 1.0;
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

    stream << std::hex << float_to_int(attribute_value.red);
    hex_string += stream.str();

    stream << std::hex << float_to_int(attribute_value.green);
    hex_string += stream.str();

    stream << std::hex << float_to_int(attribute_value.blue);
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
    // Default value of alpha in Burrito is 1.0 (i.e. 255)
    int int_alpha = 255;
    if (attribute_value.alpha != 0) {
        int_alpha = float_to_int(attribute_value.alpha);
    }

    uint32_t rgba = ((float_to_int(attribute_value.red) & 0xff) << 24) + ((float_to_int(attribute_value.green) & 0xff) << 16) + ((float_to_int(attribute_value.blue) & 0xff) << 8) + (int_alpha & 0xff);
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

    int int_red = std::stoi(hex_string.substr(0, 2), nullptr, 16);
    int int_green = std::stoi(hex_string.substr(2, 2), nullptr, 16);
    int int_blue = std::stoi(hex_string.substr(4, 2), nullptr, 16);
    int int_alpha = std::stoi(hex_string.substr(6, 2), nullptr, 16);

    color.red = int_to_float(int_red);
    color.green = int_to_float(int_green);
    color.blue = int_to_float(int_blue);
    color.alpha = int_to_float(int_alpha);

    return color;
}
