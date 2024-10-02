#pragma once

#include <cstdint>
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
class GUID;
}

class UniqueId {
 public:
    std::vector<uint8_t> guid;
};

void xml_attribute_to_unique_id(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    UniqueId* value,
    bool* is_set);

std::string unique_id_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const UniqueId* value);

void proto_to_unique_id(
    std::string input,
    ProtoReaderState* state,
    UniqueId* value,
    bool* is_set);

void unique_id_to_proto(
    UniqueId value,
    ProtoWriterState* state,
    std::function<void(std::string)> setter);
