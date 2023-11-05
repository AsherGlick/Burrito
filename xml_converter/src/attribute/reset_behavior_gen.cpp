#include "reset_behavior_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "waypoint.pb.h"

using namespace std;

void xml_attribute_to_reset_behavior(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    ResetBehavior* value,
    bool* is_set) {
    ResetBehavior reset_behavior;
    string normalized_value = normalize(get_attribute_value(input));
    if (normalized_value == "0") {
        reset_behavior = ResetBehavior::always_visible;
    }
    else if (normalized_value == "always_visible") {
        reset_behavior = ResetBehavior::always_visible;
    }
    else if (normalized_value == "1") {
        reset_behavior = ResetBehavior::map_change;
    }
    else if (normalized_value == "map_change") {
        reset_behavior = ResetBehavior::map_change;
    }
    else if (normalized_value == "2") {
        reset_behavior = ResetBehavior::daily_reset;
    }
    else if (normalized_value == "daily_reset") {
        reset_behavior = ResetBehavior::daily_reset;
    }
    else if (normalized_value == "3") {
        reset_behavior = ResetBehavior::never;
    }
    else if (normalized_value == "never") {
        reset_behavior = ResetBehavior::never;
    }
    else if (normalized_value == "4") {
        reset_behavior = ResetBehavior::timer;
    }
    else if (normalized_value == "timer") {
        reset_behavior = ResetBehavior::timer;
    }
    else if (normalized_value == "5") {
        reset_behavior = ResetBehavior::map_reset;
    }
    else if (normalized_value == "map_reset") {
        reset_behavior = ResetBehavior::map_reset;
    }
    else if (normalized_value == "6") {
        reset_behavior = ResetBehavior::instance_change;
    }
    else if (normalized_value == "instance_change") {
        reset_behavior = ResetBehavior::instance_change;
    }
    else if (normalized_value == "7") {
        reset_behavior = ResetBehavior::daily_reset_per_character;
    }
    else if (normalized_value == "daily_reset_per_character") {
        reset_behavior = ResetBehavior::daily_reset_per_character;
    }
    else if (normalized_value == "101") {
        reset_behavior = ResetBehavior::weekly_reset;
    }
    else if (normalized_value == "weekly_reset") {
        reset_behavior = ResetBehavior::weekly_reset;
    }
    else {
        errors->push_back(new XMLAttributeValueError("Found an invalid value that was not in the Enum ResetBehavior", input));
        reset_behavior = ResetBehavior::always_visible;
    }
    *value = reset_behavior;
    *is_set = true;
}

string reset_behavior_to_xml_attribute(const std::string& attribute_name, const ResetBehavior* value) {
    if (*value == ResetBehavior::always_visible) {
        return " " + attribute_name + "=\"" + "0" + "\"";
    }
    else if (*value == ResetBehavior::always_visible) {
        return " " + attribute_name + "=\"" + "always_visible" + "\"";
    }
    else if (*value == ResetBehavior::map_change) {
        return " " + attribute_name + "=\"" + "1" + "\"";
    }
    else if (*value == ResetBehavior::map_change) {
        return " " + attribute_name + "=\"" + "map_change" + "\"";
    }
    else if (*value == ResetBehavior::daily_reset) {
        return " " + attribute_name + "=\"" + "2" + "\"";
    }
    else if (*value == ResetBehavior::daily_reset) {
        return " " + attribute_name + "=\"" + "daily_reset" + "\"";
    }
    else if (*value == ResetBehavior::never) {
        return " " + attribute_name + "=\"" + "3" + "\"";
    }
    else if (*value == ResetBehavior::never) {
        return " " + attribute_name + "=\"" + "never" + "\"";
    }
    else if (*value == ResetBehavior::timer) {
        return " " + attribute_name + "=\"" + "4" + "\"";
    }
    else if (*value == ResetBehavior::timer) {
        return " " + attribute_name + "=\"" + "timer" + "\"";
    }
    else if (*value == ResetBehavior::map_reset) {
        return " " + attribute_name + "=\"" + "5" + "\"";
    }
    else if (*value == ResetBehavior::map_reset) {
        return " " + attribute_name + "=\"" + "map_reset" + "\"";
    }
    else if (*value == ResetBehavior::instance_change) {
        return " " + attribute_name + "=\"" + "6" + "\"";
    }
    else if (*value == ResetBehavior::instance_change) {
        return " " + attribute_name + "=\"" + "instance_change" + "\"";
    }
    else if (*value == ResetBehavior::daily_reset_per_character) {
        return " " + attribute_name + "=\"" + "7" + "\"";
    }
    else if (*value == ResetBehavior::daily_reset_per_character) {
        return " " + attribute_name + "=\"" + "daily_reset_per_character" + "\"";
    }
    else if (*value == ResetBehavior::weekly_reset) {
        return " " + attribute_name + "=\"" + "101" + "\"";
    }
    else if (*value == ResetBehavior::weekly_reset) {
        return " " + attribute_name + "=\"" + "weekly_reset" + "\"";
    }
    else {
        return " " + attribute_name + "=\"" + "ResetBehavior::0" + "\"";
    }
}

ResetBehavior from_proto_reset_behavior(waypoint::ResetBehavior proto_reset_behavior) {
    switch (proto_reset_behavior) {
        case waypoint::ResetBehavior::always_visible:
            return ResetBehavior::always_visible;
        case waypoint::ResetBehavior::map_change:
            return ResetBehavior::map_change;
        case waypoint::ResetBehavior::daily_reset:
            return ResetBehavior::daily_reset;
        case waypoint::ResetBehavior::never:
            return ResetBehavior::never;
        case waypoint::ResetBehavior::timer:
            return ResetBehavior::timer;
        case waypoint::ResetBehavior::map_reset:
            return ResetBehavior::map_reset;
        case waypoint::ResetBehavior::instance_change:
            return ResetBehavior::instance_change;
        case waypoint::ResetBehavior::daily_reset_per_character:
            return ResetBehavior::daily_reset_per_character;
        case waypoint::ResetBehavior::weekly_reset:
            return ResetBehavior::weekly_reset;
        default:
            return ResetBehavior::always_visible;
    }
}

void reset_behavior_to_proto(ResetBehavior value, std::function<void(waypoint::ResetBehavior)> setter) {
    switch (value) {
        case ResetBehavior::always_visible:
            setter(waypoint::ResetBehavior::always_visible);
            break;
        case ResetBehavior::map_change:
            setter(waypoint::ResetBehavior::map_change);
            break;
        case ResetBehavior::daily_reset:
            setter(waypoint::ResetBehavior::daily_reset);
            break;
        case ResetBehavior::never:
            setter(waypoint::ResetBehavior::never);
            break;
        case ResetBehavior::timer:
            setter(waypoint::ResetBehavior::timer);
            break;
        case ResetBehavior::map_reset:
            setter(waypoint::ResetBehavior::map_reset);
            break;
        case ResetBehavior::instance_change:
            setter(waypoint::ResetBehavior::instance_change);
            break;
        case ResetBehavior::daily_reset_per_character:
            setter(waypoint::ResetBehavior::daily_reset_per_character);
            break;
        case ResetBehavior::weekly_reset:
            setter(waypoint::ResetBehavior::weekly_reset);
            break;
        default:
            setter(waypoint::ResetBehavior::always_visible);
            break;
    }
}
