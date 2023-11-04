#pragma once

#include <string>
#include <vector>
#include <functional>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

void xml_attribute_to_bool(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    bool* value,
    bool* is_set);

std::string bool_to_xml_attribute(const std::string& attribute_name, const bool* value);


// Zero Cost Abstraction identity functions to make parsing and writing protobufs more uniform
inline bool const& from_proto_bool(const bool& x) {
    return x;
}

void bool_to_proto(bool value, std::function<void(bool)> setter);
