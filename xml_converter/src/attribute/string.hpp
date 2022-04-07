#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

string parse_string(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);

