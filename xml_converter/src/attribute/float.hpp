#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

float parse_float(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_float(float attribute_value);

// Zero Cost Abstraction identity functions to make parsing and writing protobufs more uniform
inline float const& from_proto_float(const float& x) {
    return x;
}
inline float const& to_proto_float(const float& x) {
    return x;
}
