#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;
namespace waypoint {
class TrailData;
}

class TrailData {
 public:
    std::vector<float> points_x;
    std::vector<float> points_y;
    std::vector<float> points_z;
};

void xml_attribute_to_trail_data(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    std::string base_dir,
    TrailData* value,
    bool* is_set,
    int* map_id_value,
    bool* is_map_id_set);

std::string trail_data_to_xml_attribute(const std::string& attribute_name, const TrailData* value);

void proto_to_trail_data(waypoint::TrailData input, TrailData* value, bool* is_set);

void trail_data_to_proto(TrailData value, std::function<void(waypoint::TrailData*)> setter);
