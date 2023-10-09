#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

bool parse_bool(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_bool(bool attribute_value);

// Zero Cost Abstraction identity functions to make parsing and writing protobufs more uniform
inline bool const& from_proto_bool(const bool& x) {
    return x;
}
inline bool const& to_proto_bool(const bool& x) {
    return x;
}
