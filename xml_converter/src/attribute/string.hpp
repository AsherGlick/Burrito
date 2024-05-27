#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/proto_reader_state.hpp"
#include "../state_structs/proto_writer_state.hpp"
#include "../state_structs/xml_reader_state.hpp"
#include "../state_structs/xml_writer_state.hpp"

class XMLError;

void xml_attribute_to_string(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    std::string* value,
    bool* is_set);

std::string string_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const std::string* value);

void proto_to_string(
    std::string input,
    ProtoReaderState* state,
    std::string* value,
    bool* is_set);

void string_to_proto(
    std::string value,
    ProtoWriterState* state,
    std::function<void(std::string)> setter);

// These do not belong here, they should be split out into attribute specific
// files. However we dont have a way to dynamically include attribute specific
// source files yet so they are going to live here until we build that out.
void proto_display_name_to_display_name_and_name(
    std::string input,
    ProtoReaderState* state,
    std::string* display_name,
    bool* is_display_name_set,
    std::string* name,
    bool* is_name_set);

void display_name_and_name_to_proto_display_name(
    std::string input,
    ProtoWriterState* state,
    std::function<void(std::string)> setter,
    const std::string* name,
    const bool* is_name_set);
