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

enum ResetBehavior {
    always_visible,
    daily_reset,
    daily_reset_per_character,
    instance_change,
    map_change,
    map_reset,
    never,
    timer,
    weekly_reset,
};
void xml_attribute_to_reset_behavior(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    ResetBehavior* value,
    bool* is_set);

std::string reset_behavior_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const ResetBehavior* value);

void proto_to_reset_behavior(
    guildpoint::ResetBehavior input,
    ProtoReaderState* state,
    ResetBehavior* value,
    bool* is_set);

void reset_behavior_to_proto(
    ResetBehavior value,
    ProtoWriterState* state,
    std::function<void(guildpoint::ResetBehavior)> setter);
