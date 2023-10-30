#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;
namespace waypoint {
class RGBAColor;
}

class Color {
 public:
    float red;
    float green;
    float blue;
    float alpha;
};

void xml_attribute_to_color(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    Color* value,
    bool* is_set);

std::string color_to_xml_attribute(const std::string& attribute_name, const Color* value);

waypoint::RGBAColor* to_proto_color(Color attribute_value);

Color from_proto_color(waypoint::RGBAColor attribute_value);
