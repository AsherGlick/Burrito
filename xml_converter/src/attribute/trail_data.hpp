#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

class XMLError;

class TrailData {
 public:
    std::string trail_data;
};

TrailData parse_trail_data(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_trail_data(TrailData attribute_value);

waypoint::TrailData* to_proto_trail_data(TrailData attribute_value);
