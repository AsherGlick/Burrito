#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;
namespace waypoint {
class TrailData;
}

class TrailData {
 public:
    std::string trail_data_relative_path;
    int side_effect_map_id;
    std::vector<float> points_x;
    std::vector<float> points_y;
    std::vector<float> points_z;
};

TrailData parse_trail_data(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors, std::string base_dir);

std::string stringify_trail_data(TrailData attribute_value);

waypoint::TrailData* to_proto_trail_data(TrailData attribute_value);

TrailData from_proto_trail_data(waypoint::TrailData attribute_value);
