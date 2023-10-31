#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

class XMLError;

enum CullChirality {
    clockwise,
    counter_clockwise,
    none,
};
CullChirality parse_cull_chirality(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);
std::string cull_chirality_to_xml_attribute(const std::string& attribute_name, const CullChirality* value);
waypoint::CullChirality to_proto_cull_chirality(CullChirality attribute_value);
CullChirality from_proto_cull_chirality(waypoint::CullChirality proto_cull_chirality);
