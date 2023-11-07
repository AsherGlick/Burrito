#pragma once

#include <functional>
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

void proto_to_cull_chirality(waypoint::CullChirality input, CullChirality* value, bool* is_set);

void cull_chirality_to_proto(CullChirality value, std::function<void(waypoint::CullChirality)> setter);
