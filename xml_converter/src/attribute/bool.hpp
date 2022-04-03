#pragma once

#include <string>
#include <stdexcept>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

bool parse_bool(rapidxml::xml_attribute<>* input, vector<string> *errors);
