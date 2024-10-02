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

namespace guildpoint {
class RGBAColor;
}

class Color {
 public:
    float red;
    float green;
    float blue;
    float alpha;
};

void xml_attribute_to_color(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    Color* value,
    bool* is_set);

std::string color_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const Color* value);

void proto_to_color(
    uint32_t input,
    ProtoReaderState* state,
    Color* value,
    bool* is_set);

void color_to_proto(
    Color value,
    ProtoWriterState* state,
    std::function<void(uint32_t)> setter);
