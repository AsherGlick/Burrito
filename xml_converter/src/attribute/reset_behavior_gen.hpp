#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
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
ResetBehavior parse_reset_behavior(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);
std::string reset_behavior_to_xml_attribute(const std::string& attribute_name, const ResetBehavior* value);
waypoint::ResetBehavior to_proto_reset_behavior(ResetBehavior attribute_value);
ResetBehavior from_proto_reset_behavior(waypoint::ResetBehavior proto_reset_behavior);
