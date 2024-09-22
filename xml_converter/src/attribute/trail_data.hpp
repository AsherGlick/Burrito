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

class TrailData {
 public:
    std::vector<float> points_x;
    std::vector<float> points_y;
    std::vector<float> points_z;
};

void xml_attribute_to_trail_data(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    TrailData* value,
    bool* is_set,
    int* map_id_value,
    bool* is_map_id_set);

std::string trail_data_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const TrailData* value,
    const int* map_id_value,
    const bool* is_map_id_set);

void proto_to_trail_data(
    guildpoint::TrailData input,
    ProtoReaderState* state,
    TrailData* value,
    bool* is_set);

void trail_data_to_proto(
    TrailData value,
    ProtoWriterState* state,
    std::function<void(guildpoint::TrailData*)> setter);
