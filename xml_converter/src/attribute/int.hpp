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

void xml_attribute_to_int(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    int* value,
    bool* is_set);

void int_to_xml_attribute(
    XMLWriterState* state,
    const int* value,
    std::function<void(std::string)> setter);

void proto_to_int(
    int input,
    ProtoReaderState* state,
    int* value,
    bool* is_set);

void int_to_proto(
    int value,
    ProtoWriterState* state,
    std::function<void(int&)> setter);
