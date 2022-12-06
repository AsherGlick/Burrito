#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

class XMLError;

bool parse_bool(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_bool(bool attribute_value);

bool to_proto_bool (bool attribute_value);

bool from_proto_bool (bool attribute_value);
