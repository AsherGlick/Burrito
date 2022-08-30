#pragma once

#include<string>
#include<vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

float parse_float(rapidxml::xml_attribute<>* input, std::vector<XMLError*> *errors);

string stringify_float (float attribute_value);