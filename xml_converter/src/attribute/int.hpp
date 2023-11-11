#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/xml_parse_state.hpp"

class XMLError;

void xml_attribute_to_int(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLParseState* state,
    int* value,
    bool* is_set);

std::string int_to_xml_attribute(const std::string& attribute_name, const int* value);

void proto_to_int(int input, int* value, bool* is_set);

void int_to_proto(int value, std::function<void(int&)> setter);
