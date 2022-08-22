#include "reset_behavior.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

ResetBehavior parse_reset_behavior(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
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
	return reset_behavior;
}