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

namespace Attribute::Color {
class Color {
 public:
    float red;
    float green;
    float blue;
    float alpha;
};

void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    Color* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const Color* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    uint32_t input,
    ProtoReaderState* state,
    Color* value,
    bool* is_set
);

void to_proto_field(
    Color value,
    ProtoWriterState* state,
    std::function<void(uint32_t)> setter
);
}  // namespace Attribute::Color
