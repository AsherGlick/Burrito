#pragma once

#include <string>
#include <stdexcept>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

bool parse_bool(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);

string stringify_bool(bool attribute_value);
