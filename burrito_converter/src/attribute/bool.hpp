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

namespace Attribute::Bool {

void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    bool* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const bool* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    bool input,
    ProtoReaderState* state,
    bool* value,
    bool* is_set
);

void to_proto_field(
    bool value,
    ProtoWriterState* state,
    std::function<void(bool)> setter
);

}  // namespace Attribute::Bool

namespace Attribute::InvertBool {

void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    bool* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const bool* value,
    std::function<void(std::string)> setter
);

}  // namespace Attribute::InvertBool
