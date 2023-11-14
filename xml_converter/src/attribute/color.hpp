#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/xml_parse_state.hpp"

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
    XMLParseState* state,
    Color* value,
    bool* is_set);

std::string color_to_xml_attribute(const std::string& attribute_name, const Color* value);

void proto_to_color(waypoint::RGBAColor input, Color* value, bool* is_set);

void color_to_proto(Color value, std::function<void(waypoint::RGBAColor*)> setter);
