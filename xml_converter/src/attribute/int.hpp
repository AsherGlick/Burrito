#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

void xml_attribute_to_int(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    int* value,
    bool* is_set);

std::string int_to_xml_attribute(const std::string& attribute_name, const int* value);

// Zero Cost Abstraction identity functions to make parsing and writing protobufs more uniform
inline int const& from_proto_int(const int& x) {
    return x;
}
inline int const& to_proto_int(const int& x) {
    return x;
}
