#pragma once

#include <string>
#include <vector>
#include <functional>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

void xml_attribute_to_float(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    float* value,
    bool* is_set);

std::string float_to_xml_attribute(const std::string& attribute_name, const float* value);

// Zero Cost Abstraction identity functions to make parsing and writing protobufs more uniform
inline float const& from_proto_float(const float& x) {
    return x;
}

void float_to_proto(float value, std::function<void(float&)> setter);
