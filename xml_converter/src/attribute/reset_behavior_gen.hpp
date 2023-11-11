#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/xml_parse_state.hpp"

#include "waypoint.pb.h"

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
    XMLParseState* state,
    ResetBehavior* value,
    bool* is_set);

std::string reset_behavior_to_xml_attribute(const std::string& attribute_name, const ResetBehavior* value);

void proto_to_reset_behavior(waypoint::ResetBehavior input, ResetBehavior* value, bool* is_set);

void reset_behavior_to_proto(ResetBehavior value, std::function<void(waypoint::ResetBehavior)> setter);
