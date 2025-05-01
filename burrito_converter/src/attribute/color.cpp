#include "color.hpp"

#include <stdint.h>

#include <algorithm>
#include <iosfwd>
#include <sstream>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "guildpoint.pb.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// convert_color_channel_int_to_float
//
// Helper function to convert the value of a color from an 8bit integer to a
// floating point number. Numbers outside of the valid range will be clamped.
////////////////////////////////////////////////////////////////////////////////
static float convert_color_channel_int_to_float(int input) {
    if (input > 255) {
        input = 255;
    }
    if (input < 0) {
        input = 0;
    }
    float output = static_cast<float>(input) / 255.0f;
    return output;
}

////////////////////////////////////////////////////////////////////////////////
// convert_color_channel_float_to_int
//
// Helper function to convert the value of a color from a floating point number
// to an 8bit integer. Numbers outside of the valid range will be clamped.
////////////////////////////////////////////////////////////////////////////////
static int convert_color_channel_float_to_int(float input) {
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
// from_xml_attribute
//
// Reads a Color from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Color::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    Color* value,
    bool* is_set
) {
    Color color;
    std::string input_string = get_attribute_value(input);
    std::string hex_string;

    if (input_string.size() > 0 && input_string[0] == '#') {
        hex_string = input_string.substr(1);
    }
    else {
        hex_string = input_string;
    }

    if (is_hex(hex_string)) {
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
// to_xml_attribute
//
// Writes a Color to an xml attribute using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Color::to_xml_attribute(
    XMLWriterState*,
    const Color* value,
    std::function<void(std::string)> setter
) {
    std::stringstream stream;
    std::string hex_string = "#";

    stream << std::hex << convert_color_channel_float_to_int((*value).red);
    hex_string += stream.str();

    stream << std::hex << convert_color_channel_float_to_int((*value).green);
    hex_string += stream.str();

    stream << std::hex << convert_color_channel_float_to_int((*value).blue);
    hex_string += stream.str();

    std::string rgb = hex_string;

    setter(rgb);
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads a Color from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Color::from_proto_field(
    uint32_t input,
    ProtoReaderState*,
    Color* value,
    bool* is_set
) {
    Color color;
    std::stringstream stream;

    color.red = convert_color_channel_int_to_float((input >> 24) & 0xff);
    color.green = convert_color_channel_int_to_float((input >> 16) & 0xff);
    color.blue = convert_color_channel_int_to_float((input >> 8) & 0xff);
    color.alpha = convert_color_channel_int_to_float(input & 0xff);

    *value = color;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes a Color to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Color::to_proto_field(
    Color value,
    ProtoWriterState*,
    std::function<void(uint32_t)> setter
) {
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

    setter(rgba);
}
