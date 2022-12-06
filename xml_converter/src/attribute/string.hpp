#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

class XMLError;

std::string parse_string(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_string(std::string attribute_value);

std::string to_proto_string(std::string attribute_value);

std::string from_proto_string(std::string attribute_value);
