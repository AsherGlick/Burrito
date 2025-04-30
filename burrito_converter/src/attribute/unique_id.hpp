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
#include "../string_helper.hpp"

class XMLError;

namespace guildpoint {
class GUID;
}

namespace Attribute::UniqueId {

class UniqueId {
 public:
    std::vector<uint8_t> guid;

    bool operator<(const UniqueId& other) const {
        return guid < other.guid;
    }

    std::string get_value_as_base_64() const {
        return base64_encode(&(guid[0]), guid.size());
    }
};

void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    UniqueId* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const UniqueId* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    std::string input,
    ProtoReaderState* state,
    UniqueId* value,
    bool* is_set
);

void to_proto_field(
    UniqueId value,
    ProtoWriterState* state,
    std::function<void(std::string)> setter
);

}  // namespace Attribute::UniqueId
