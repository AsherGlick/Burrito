#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

bool parse_bool(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_bool(bool attribute_value);
