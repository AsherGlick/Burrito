#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

void xml_attribute_to_bool(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    bool* value,
    bool* is_set);

std::string bool_to_xml_attribute(
    const std::string& attribute_name,
    const bool* value);

void inverted_xml_attribute_to_bool(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    bool* value,
    bool* is_set);

std::string bool_to_inverted_xml_attribute(
    const std::string& attribute_name,
    const bool* value);

void proto_to_bool(bool input, bool* value, bool* is_set);

void bool_to_proto(bool value, std::function<void(bool)> setter);
