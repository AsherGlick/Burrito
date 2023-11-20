#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/xml_reader_state.hpp"

class XMLError;

void xml_attribute_to_string(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    std::string* value,
    bool* is_set);

std::string string_to_xml_attribute(const std::string& attribute_name, const std::string* value);

void proto_to_string(std::string input, std::string* value, bool* is_set);

void string_to_proto(std::string value, std::function<void(std::string)> setter);

// These do not belong here, they should be split out into attribute specific
// files. However we dont have a way to dynamically include attribute specific
// source files yet so they are going to live here until we build that out.
void proto_display_name_to_display_name_and_name(std::string input, std::string* display_name, bool* is_display_name_set, std::string* name, bool* is_name_set);
#define do_nothing(...)
