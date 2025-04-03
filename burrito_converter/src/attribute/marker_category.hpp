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
class Category;
}

namespace Attribute::MarkerCategory {

class MarkerCategory {
 public:
    std::string category;
};

void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    MarkerCategory* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const MarkerCategory* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    guildpoint::Category input,
    ProtoReaderState* state,
    MarkerCategory* value,
    bool* is_set
);

void to_proto_field(
    MarkerCategory value,
    ProtoWriterState* state,
    std::function<void(guildpoint::Category*)> setter
);

}  // namespace Attribute::MarkerCategory
