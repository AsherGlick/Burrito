#include "color.hpp"

#include <stdint.h>

#include <algorithm>
#include <iosfwd>
#include <ostream>
#include <regex>
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
float convert_color_channel_int_to_float(int input) {
    if (input > 255) {
        input = 255;
    }
    if (input < 0) {
        input = 0;
    }
    float output = static_cast<float>(input) / 255.0f;
    return output;
}

int convert_color_channel_float_to_int(float input) {
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
////////////////////////////////////////////////////////////////////////////////
void xml_attribute_to_color(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    Color* value,
    bool* is_set) {
    Color color;
    std::string input_string = get_attribute_value(input);
    std::string hex_string;

    if (input_string.size() > 0 && input_string[0] == '#') {
        hex_string = input_string.substr(1);
    }
    else {
        hex_string = input_string;
    }

    std::regex hex_pattern("^([A-Fa-f0-9]+)");

    if (std::regex_match(hex_string, hex_pattern)) {
        // Extract the R, G, B, and A values from the Hex string
        if (hex_string.size() == 6 || hex_string.size() == 8) {
            color.red = convert_color_channel_int_to_float(std::stoi(hex_string.substr(0, 2), nullptr, 16));
            color.green = convert_color_channel_int_to_float(std::stoi(hex_string.substr(2, 2), nullptr, 16));
            color.blue = convert_color_channel_int_to_float(std::stoi(hex_string.substr(4, 2), nullptr, 16));
            if (hex_string.size() == 8) {
                color.alpha = convert_color_channel_int_to_float(std::stoi(hex_string.substr(6, 2), nullptr, 16));
            }
            else {
                color.alpha = 1.0;
            }
        }
        else {
            errors->push_back(new XMLAttributeValueError("Found a hex color value that was not 6 or 8 characters", input));
        }
    }
    else {
        errors->push_back(new XMLAttributeValueError("Found a color value not in hex format", input));
    }
    *value = color;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// color_to_xml_attribute
//
// Converts a color into a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
string color_to_xml_attribute(
    const string& attribute_name,
    XMLWriterState* state,
    const Color* value) {
    std::stringstream stream;
    std::string hex_string = "#";

    stream << std::hex << convert_color_channel_float_to_int((*value).red);
    hex_string += stream.str();

    stream << std::hex << convert_color_channel_float_to_int((*value).green);
    hex_string += stream.str();

    stream << std::hex << convert_color_channel_float_to_int((*value).blue);
    hex_string += stream.str();

    std::string rgb = hex_string;

    return " " + attribute_name + "=\"" + rgb + "\"";
}

////////////////////////////////////////////////////////////////////////////////
// proto_to_color
//
// Parses a Color from a proto field.
////////////////////////////////////////////////////////////////////////////////
void proto_to_color(
    waypoint::RGBAColor input,
    ProtoReaderState* state,
    Color* value,
    bool* is_set) {
    Color color;
    std::stringstream stream;
    uint32_t rgba = input.rgba_color();

    color.red = convert_color_channel_int_to_float((rgba >> 24) & 0xff);
    color.green = convert_color_channel_int_to_float((rgba >> 16) & 0xff);
    color.blue = convert_color_channel_int_to_float((rgba >> 8) & 0xff);
    color.alpha = convert_color_channel_int_to_float(rgba & 0xff);

    *value = color;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// color_to_proto
//
// Writes a Color to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void color_to_proto(
    Color value,
    ProtoWriterState* state,
    std::function<void(waypoint::RGBAColor*)> setter) {
    waypoint::RGBAColor* color = new waypoint::RGBAColor();
    // The default RGB in burrito will be 000000 (i.e. black)
    // Default value of alpha in Burrito is 1.0 (i.e. 255)
    int int_alpha = 255;
    // If alpha (float) is not the default value, convert to int
    if (value.alpha != 0) {
        int_alpha = convert_color_channel_float_to_int(value.alpha);
    }

    uint32_t r = ((convert_color_channel_float_to_int(value.red) & 0xff) << 24);
    uint32_t g = ((convert_color_channel_float_to_int(value.green) & 0xff) << 16);
    uint32_t b = ((convert_color_channel_float_to_int(value.blue) & 0xff) << 8);
    uint32_t a = (int_alpha & 0xff);
    uint32_t rgba = r | g | b | a;

    color->set_rgba_color(rgba);
    setter(color);
}
