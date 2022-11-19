#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

int parse_int(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_int(int attribute_value);
