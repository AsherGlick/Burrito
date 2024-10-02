#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/proto_reader_state.hpp"
#include "../state_structs/proto_writer_state.hpp"
#include "../state_structs/xml_reader_state.hpp"
#include "../state_structs/xml_writer_state.hpp"
#include "guildpoint.pb.h"

class XMLError;

enum CullChirality {
    clockwise,
    counter_clockwise,
    none,
};
void xml_attribute_to_cull_chirality(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    CullChirality* value,
    bool* is_set);

std::string cull_chirality_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const CullChirality* value);

void proto_to_cull_chirality(
    guildpoint::CullChirality input,
    ProtoReaderState* state,
    CullChirality* value,
    bool* is_set);

void cull_chirality_to_proto(
    CullChirality value,
    ProtoWriterState* state,
    std::function<void(guildpoint::CullChirality)> setter);
