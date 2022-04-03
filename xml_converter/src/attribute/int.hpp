#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

int parse_int(rapidxml::xml_attribute<>* input, vector<string> *errors);

int init_int_attribute();
