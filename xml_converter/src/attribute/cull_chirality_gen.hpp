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
void xml_attribute_to_cull_chirality(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    CullChirality* value,
    bool* is_set);
std::string cull_chirality_to_xml_attribute(const std::string& attribute_name, const CullChirality* value);
waypoint::CullChirality to_proto_cull_chirality(CullChirality attribute_value);
CullChirality from_proto_cull_chirality(waypoint::CullChirality proto_cull_chirality);
