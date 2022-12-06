#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

float parse_float(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_float(float attribute_value);

float to_proto_float(float attribute_value);

float from_proto_float(float attribute_value);
