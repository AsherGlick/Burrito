#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

class XMLError;

class TrailDataMapId {
 public:
    int trail_data_map_id;
};

TrailDataMapId parse_trail_data_map_id(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_trail_data_map_id(TrailDataMapId attribute_value);

// waypoint::TrailDataMapID* to_proto_trail_data(TrailDataMapID attribute_value);
