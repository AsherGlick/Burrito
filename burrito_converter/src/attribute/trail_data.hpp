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
class TrailData;
}

namespace Attribute::TrailData {

struct XYZ {
    float x;
    float y;
    float z;
};

class TrailData {
 public:
    std::vector<XYZ> points;
};

void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    TrailData* value,
    bool* is_set,
    int* map_id_value,
    bool* is_map_id_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const TrailData* value,
    const int* map_id_value,
    const bool* is_map_id_set,
    std::function<void(std::string)> setter
);

void from_proto_field(
    guildpoint::TrailData input,
    ProtoReaderState* state,
    TrailData* value,
    bool* is_set
);

void to_proto_field(
    TrailData value,
    ProtoWriterState* state,
    std::function<void(guildpoint::TrailData*)> setter
);
}  // namespace Attribute::TrailData
