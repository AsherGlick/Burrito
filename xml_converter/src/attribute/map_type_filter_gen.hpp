#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
class XMLError;
namespace waypoint {
class MapTypeFilter;
}

class MapTypeFilter {
 public:
    bool blue_borderlands_map;
    bool bluehome_map;
    bool center_map;
    bool character_create_map;
    bool edge_of_the_mists_map;
    bool eternal_battlegrounds_map;
    bool fortunes_vale_map;
    bool green_borderlands_map;
    bool green_home_map;
    bool gvg_map;
    bool instance_map;
    bool jump_puzzle_map;
    bool obsidian_sanctum_map;
    bool public_map;
    bool public_mini_map;
    bool pvp_map;
    bool red_borderlands_map;
    bool red_home_map;
    bool redirect_map;
    bool tournament_map;
    bool tutorial_map;
    bool unknown_map;
    bool user_tournament_map;
    bool wvw_lounge_map;

    virtual std::string classname() {
        return "MapTypeFilter";
    }
};
void xml_attribute_to_map_type_filter(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    MapTypeFilter* value,
    bool* is_set);
std::string map_type_filter_to_xml_attribute(const std::string& attribute_name, const MapTypeFilter* value);

MapTypeFilter from_proto_map_type_filter(waypoint::MapTypeFilter proto_map_type_filter);

void map_type_filter_to_proto(MapTypeFilter value, std::function<void(waypoint::MapTypeFilter*)> setter);
