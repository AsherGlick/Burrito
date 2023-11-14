#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/xml_parse_state.hpp"

class XMLError;

void xml_attribute_to_float(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLParseState* state,
    float* value,
    bool* is_set);

std::string float_to_xml_attribute(const std::string& attribute_name, const float* value);

void proto_to_float(float input, float* value, bool* is_set);

void float_to_proto(float value, std::function<void(float&)> setter);
