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

namespace Attribute::Float {

void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    float* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const float* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    float input,
    ProtoReaderState* state,
    float* value,
    bool* is_set
);

void to_proto_field(
    float value,
    ProtoWriterState* state,
    std::function<void(float&)> setter
);

}  // namespace Attribute::Float
