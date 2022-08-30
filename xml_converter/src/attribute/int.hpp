#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

int parse_int(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);

int init_int_attribute();

string stringify_int (int attribute_value);