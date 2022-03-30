#pragma once

#include "../rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>

using namespace std;

string parse_string(rapidxml::xml_attribute<>* input, vector<string> *errors);

