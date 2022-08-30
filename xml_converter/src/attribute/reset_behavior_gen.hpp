#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;
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
ResetBehavior parse_reset_behavior(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
string stringify_reset_behavior(ResetBehavior attribute_value);