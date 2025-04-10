#include "reset_behavior_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "guildpoint.pb.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads a ResetBehavior from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::ResetBehavior::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    ResetBehavior* value,
    bool* is_set
) {
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

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Writes a ResetBehavior to an xml attribute using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::ResetBehavior::to_xml_attribute(
    XMLWriterState*,
    const ResetBehavior* value,
    std::function<void(std::string)> setter
) {
    if (*value == ResetBehavior::always_visible) {
        setter("0");
        return;
    }
    else if (*value == ResetBehavior::always_visible) {
        setter("always_visible");
        return;
    }
    else if (*value == ResetBehavior::map_change) {
        setter("1");
        return;
    }
    else if (*value == ResetBehavior::map_change) {
        setter("map_change");
        return;
    }
    else if (*value == ResetBehavior::daily_reset) {
        setter("2");
        return;
    }
    else if (*value == ResetBehavior::daily_reset) {
        setter("daily_reset");
        return;
    }
    else if (*value == ResetBehavior::never) {
        setter("3");
        return;
    }
    else if (*value == ResetBehavior::never) {
        setter("never");
        return;
    }
    else if (*value == ResetBehavior::timer) {
        setter("4");
        return;
    }
    else if (*value == ResetBehavior::timer) {
        setter("timer");
        return;
    }
    else if (*value == ResetBehavior::map_reset) {
        setter("5");
        return;
    }
    else if (*value == ResetBehavior::map_reset) {
        setter("map_reset");
        return;
    }
    else if (*value == ResetBehavior::instance_change) {
        setter("6");
        return;
    }
    else if (*value == ResetBehavior::instance_change) {
        setter("instance_change");
        return;
    }
    else if (*value == ResetBehavior::daily_reset_per_character) {
        setter("7");
        return;
    }
    else if (*value == ResetBehavior::daily_reset_per_character) {
        setter("daily_reset_per_character");
        return;
    }
    else if (*value == ResetBehavior::weekly_reset) {
        setter("101");
        return;
    }
    else if (*value == ResetBehavior::weekly_reset) {
        setter("weekly_reset");
        return;
    }
    else {
        setter("ResetBehavior::0");
    }
}
