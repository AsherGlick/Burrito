#include "reset_behavior.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

ResetBehavior parse_reset_behavior(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
	ResetBehavior reset_behavior;
	if (get_attribute_value(input) == "0") {
		reset_behavior = ResetBehavior::always_visible;
	}
	else if (get_attribute_value(input) == "always_visible") {
		reset_behavior = ResetBehavior::always_visible;
	}
	else if (get_attribute_value(input) == "1") {
		reset_behavior = ResetBehavior::map_change;
	}
	else if (get_attribute_value(input) == "map_change") {
		reset_behavior = ResetBehavior::map_change;
	}
	else if (get_attribute_value(input) == "2") {
		reset_behavior = ResetBehavior::daily_reset;
	}
	else if (get_attribute_value(input) == "daily_reset") {
		reset_behavior = ResetBehavior::daily_reset;
	}
	else if (get_attribute_value(input) == "3") {
		reset_behavior = ResetBehavior::never;
	}
	else if (get_attribute_value(input) == "never") {
		reset_behavior = ResetBehavior::never;
	}
	else if (get_attribute_value(input) == "4") {
		reset_behavior = ResetBehavior::timer;
	}
	else if (get_attribute_value(input) == "timer") {
		reset_behavior = ResetBehavior::timer;
	}
	else if (get_attribute_value(input) == "5") {
		reset_behavior = ResetBehavior::map_reset;
	}
	else if (get_attribute_value(input) == "map_reset") {
		reset_behavior = ResetBehavior::map_reset;
	}
	else if (get_attribute_value(input) == "6") {
		reset_behavior = ResetBehavior::instance_change;
	}
	else if (get_attribute_value(input) == "instance_change") {
		reset_behavior = ResetBehavior::instance_change;
	}
	else if (get_attribute_value(input) == "7") {
		reset_behavior = ResetBehavior::daily_reset_per_character;
	}
	else if (get_attribute_value(input) == "daily_reset_per_character") {
		reset_behavior = ResetBehavior::daily_reset_per_character;
	}
	else if (get_attribute_value(input) == "101") {
		reset_behavior = ResetBehavior::weekly_reset;
	}
	else if (get_attribute_value(input) == "weekly_reset") {
		reset_behavior = ResetBehavior::weekly_reset;
	}
	else {errors->push_back(new XMLAttributeValueError("Found a value that was not in the Enum", input));
        reset_behavior = ResetBehavior::always_visible;
    }
	return reset_behavior;
}