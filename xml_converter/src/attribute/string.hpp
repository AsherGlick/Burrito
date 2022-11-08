#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

std::string parse_string(rapidxml::xml_attribute<>* input, std::vector<XMLError*> *errors);

std::string stringify_string(std::string attribute_value);