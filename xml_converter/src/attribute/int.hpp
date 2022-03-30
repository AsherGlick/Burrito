#pragma once

#include "../rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>

using namespace std;

int parse_int(rapidxml::xml_attribute<>* input, vector<string> *errors);

int init_int_attribute();