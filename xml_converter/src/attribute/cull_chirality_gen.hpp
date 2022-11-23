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
std::string stringify_cull_chirality(CullChirality attribute_value);
// waypoint::CullChirality* to_proto_cull_chirality(CullChirality attribute_value);
