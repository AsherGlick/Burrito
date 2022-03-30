#pragma once

#include "../rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <stdexcept>
#include <vector>

using namespace std;

bool parse_bool(rapidxml::xml_attribute<>* input, vector<string> *errors);
