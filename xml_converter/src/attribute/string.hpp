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

namespace Attribute::String {

void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    std::string* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const std::string* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    std::string input,
    ProtoReaderState* state,
    std::string* value,
    bool* is_set
);

void to_proto_field(
    std::string value,
    ProtoWriterState* state,
    std::function<void(std::string)> setter
);
}  // namespace Attribute::String

namespace Attribute::NameAndDisplayname {
// These do not belong here, they should be split out into attribute specific
// files. However we dont have a way to dynamically include attribute specific
// source files yet so they are going to live here until we build that out.
void from_proto_field(
    std::string input,
    ProtoReaderState* state,
    std::string* display_name,
    bool* is_display_name_set,
    std::string* name,
    bool* is_name_set
);

void to_proto_field(
    std::string input,
    ProtoWriterState* state,
    std::function<void(std::string)> setter,
    const std::string* name,
    const bool* is_name_set
);

}  // namespace Attribute::NameAndDisplayname
