#include "map_type_filter_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "waypoint.pb.h"

using namespace std;

MapTypeFilter parse_map_type_filter(rapidxml::xml_attribute<>* input, vector<XMLError*>* errors) {
    MapTypeFilter map_type_filter;
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");
    map_type_filter.unknown_map = false;
    map_type_filter.redirect_map = false;
    map_type_filter.character_create_map = false;
    map_type_filter.pvp_map = false;
    map_type_filter.gvg_map = false;
    map_type_filter.instance_map = false;
    map_type_filter.public_map = false;
    map_type_filter.tournament_map = false;
    map_type_filter.tutorial_map = false;
    map_type_filter.user_tournament_map = false;
    map_type_filter.center_map = false;
    map_type_filter.eternal_battlegrounds_map = false;
    map_type_filter.bluehome_map = false;
    map_type_filter.blue_borderlands_map = false;
    map_type_filter.green_home_map = false;
    map_type_filter.green_borderlands_map = false;
    map_type_filter.red_home_map = false;
    map_type_filter.red_borderlands_map = false;
    map_type_filter.fortunes_vale_map = false;
    map_type_filter.jump_puzzle_map = false;
    map_type_filter.obsidian_sanctum_map = false;
    map_type_filter.edge_of_the_mists_map = false;
    map_type_filter.public_mini_map = false;
    map_type_filter.wvw_lounge_map = false;

    for (string flag_value : flag_values) {
        string normalized_flag_value = normalize(flag_value);
        if (normalized_flag_value == "unknown") {
            map_type_filter.unknown_map = true;
        }
        else if (normalized_flag_value == "redirect") {
            map_type_filter.redirect_map = true;
        }
        else if (normalized_flag_value == "charactercreate") {
            map_type_filter.character_create_map = true;
        }
        else if (normalized_flag_value == "pvp") {
            map_type_filter.pvp_map = true;
        }
        else if (normalized_flag_value == "gvg") {
            map_type_filter.gvg_map = true;
        }
        else if (normalized_flag_value == "instance") {
            map_type_filter.instance_map = true;
        }
        else if (normalized_flag_value == "public") {
            map_type_filter.public_map = true;
        }
        else if (normalized_flag_value == "tournament") {
            map_type_filter.tournament_map = true;
        }
        else if (normalized_flag_value == "tutorial") {
            map_type_filter.tutorial_map = true;
        }
        else if (normalized_flag_value == "usertournament") {
            map_type_filter.user_tournament_map = true;
        }
        else if (normalized_flag_value == "center") {
            map_type_filter.center_map = true;
        }
        else if (normalized_flag_value == "eternalbattlegrounds") {
            map_type_filter.eternal_battlegrounds_map = true;
        }
        else if (normalized_flag_value == "bluehome") {
            map_type_filter.bluehome_map = true;
        }
        else if (normalized_flag_value == "blueborderlands") {
            map_type_filter.blue_borderlands_map = true;
        }
        else if (normalized_flag_value == "greenhome") {
            map_type_filter.green_home_map = true;
        }
        else if (normalized_flag_value == "greenborderlands") {
            map_type_filter.green_borderlands_map = true;
        }
        else if (normalized_flag_value == "redhome") {
            map_type_filter.red_home_map = true;
        }
        else if (normalized_flag_value == "redborderlands") {
            map_type_filter.red_borderlands_map = true;
        }
        else if (normalized_flag_value == "fortunesvale") {
            map_type_filter.fortunes_vale_map = true;
        }
        else if (normalized_flag_value == "jumppuzzle") {
            map_type_filter.jump_puzzle_map = true;
        }
        else if (normalized_flag_value == "obsidiansanctum") {
            map_type_filter.obsidian_sanctum_map = true;
        }
        else if (normalized_flag_value == "edgeofthemists") {
            map_type_filter.edge_of_the_mists_map = true;
        }
        else if (normalized_flag_value == "publicmini") {
            map_type_filter.public_mini_map = true;
        }
        else if (normalized_flag_value == "wvwlounge") {
            map_type_filter.wvw_lounge_map = true;
        }
        else {
            errors->push_back(new XMLAttributeValueError("Invalid Filter for MapTypeFilter. Found " + flag_value, input));
            continue;
        }
    }
    return map_type_filter;
}

string stringify_map_type_filter(MapTypeFilter attribute_value) {
    vector<string> flag_values;
    if (attribute_value.unknown_map == true) {
        flag_values.push_back("unknown");
    }
    if (attribute_value.redirect_map == true) {
        flag_values.push_back("redirect");
    }
    if (attribute_value.character_create_map == true) {
        flag_values.push_back("charactercreate");
    }
    if (attribute_value.pvp_map == true) {
        flag_values.push_back("pvp");
    }
    if (attribute_value.gvg_map == true) {
        flag_values.push_back("gvg");
    }
    if (attribute_value.instance_map == true) {
        flag_values.push_back("instance");
    }
    if (attribute_value.public_map == true) {
        flag_values.push_back("public");
    }
    if (attribute_value.tournament_map == true) {
        flag_values.push_back("tournament");
    }
    if (attribute_value.tutorial_map == true) {
        flag_values.push_back("tutorial");
    }
    if (attribute_value.user_tournament_map == true) {
        flag_values.push_back("usertournament");
    }
    if (attribute_value.center_map == true) {
        flag_values.push_back("center");
    }
    if (attribute_value.eternal_battlegrounds_map == true) {
        flag_values.push_back("eternalbattlegrounds");
    }
    if (attribute_value.bluehome_map == true) {
        flag_values.push_back("bluehome");
    }
    if (attribute_value.blue_borderlands_map == true) {
        flag_values.push_back("blueborderlands");
    }
    if (attribute_value.green_home_map == true) {
        flag_values.push_back("greenhome");
    }
    if (attribute_value.green_borderlands_map == true) {
        flag_values.push_back("greenborderlands");
    }
    if (attribute_value.red_home_map == true) {
        flag_values.push_back("redhome");
    }
    if (attribute_value.red_borderlands_map == true) {
        flag_values.push_back("redborderlands");
    }
    if (attribute_value.fortunes_vale_map == true) {
        flag_values.push_back("fortunesvale");
    }
    if (attribute_value.jump_puzzle_map == true) {
        flag_values.push_back("jumppuzzle");
    }
    if (attribute_value.obsidian_sanctum_map == true) {
        flag_values.push_back("obsidiansanctum");
    }
    if (attribute_value.edge_of_the_mists_map == true) {
        flag_values.push_back("edgeofthemists");
    }
    if (attribute_value.public_mini_map == true) {
        flag_values.push_back("publicmini");
    }
    if (attribute_value.wvw_lounge_map == true) {
        flag_values.push_back("wvwlounge");
    }
    string output = "";
    for (size_t i = 0; i < flag_values.size(); ++i) {
        output += flag_values[i];
        if (i < flag_values.size() - 1){
            output += ",";
        }
    }
    return output;
}

waypoint::MapTypeFilter* to_proto_map_type_filter(MapTypeFilter attribute_value) {
    waypoint::MapTypeFilter* proto_map_type_filter = new waypoint::MapTypeFilter();
    proto_map_type_filter->set_unknown_map(attribute_value.unknown_map);
    proto_map_type_filter->set_redirect_map(attribute_value.redirect_map);
    proto_map_type_filter->set_character_create_map(attribute_value.character_create_map);
    proto_map_type_filter->set_pvp_map(attribute_value.pvp_map);
    proto_map_type_filter->set_gvg_map(attribute_value.gvg_map);
    proto_map_type_filter->set_instance_map(attribute_value.instance_map);
    proto_map_type_filter->set_public_map(attribute_value.public_map);
    proto_map_type_filter->set_tournament_map(attribute_value.tournament_map);
    proto_map_type_filter->set_tutorial_map(attribute_value.tutorial_map);
    proto_map_type_filter->set_user_tournament_map(attribute_value.user_tournament_map);
    proto_map_type_filter->set_center_map(attribute_value.center_map);
    proto_map_type_filter->set_eternal_battlegrounds_map(attribute_value.eternal_battlegrounds_map);
    proto_map_type_filter->set_bluehome_map(attribute_value.bluehome_map);
    proto_map_type_filter->set_blue_borderlands_map(attribute_value.blue_borderlands_map);
    proto_map_type_filter->set_green_home_map(attribute_value.green_home_map);
    proto_map_type_filter->set_green_borderlands_map(attribute_value.green_borderlands_map);
    proto_map_type_filter->set_red_home_map(attribute_value.red_home_map);
    proto_map_type_filter->set_red_borderlands_map(attribute_value.red_borderlands_map);
    proto_map_type_filter->set_fortunes_vale_map(attribute_value.fortunes_vale_map);
    proto_map_type_filter->set_jump_puzzle_map(attribute_value.jump_puzzle_map);
    proto_map_type_filter->set_obsidian_sanctum_map(attribute_value.obsidian_sanctum_map);
    proto_map_type_filter->set_edge_of_the_mists_map(attribute_value.edge_of_the_mists_map);
    proto_map_type_filter->set_public_mini_map(attribute_value.public_mini_map);
    proto_map_type_filter->set_wvw_lounge_map(attribute_value.wvw_lounge_map);
    return proto_map_type_filter;
}

MapTypeFilter from_proto_map_type_filter(waypoint::MapTypeFilter proto_map_type_filter) {
    MapTypeFilter map_type_filter;
    map_type_filter.unknown_map = proto_map_type_filter.unknown_map();
    map_type_filter.redirect_map = proto_map_type_filter.redirect_map();
    map_type_filter.character_create_map = proto_map_type_filter.character_create_map();
    map_type_filter.pvp_map = proto_map_type_filter.pvp_map();
    map_type_filter.gvg_map = proto_map_type_filter.gvg_map();
    map_type_filter.instance_map = proto_map_type_filter.instance_map();
    map_type_filter.public_map = proto_map_type_filter.public_map();
    map_type_filter.tournament_map = proto_map_type_filter.tournament_map();
    map_type_filter.tutorial_map = proto_map_type_filter.tutorial_map();
    map_type_filter.user_tournament_map = proto_map_type_filter.user_tournament_map();
    map_type_filter.center_map = proto_map_type_filter.center_map();
    map_type_filter.eternal_battlegrounds_map = proto_map_type_filter.eternal_battlegrounds_map();
    map_type_filter.bluehome_map = proto_map_type_filter.bluehome_map();
    map_type_filter.blue_borderlands_map = proto_map_type_filter.blue_borderlands_map();
    map_type_filter.green_home_map = proto_map_type_filter.green_home_map();
    map_type_filter.green_borderlands_map = proto_map_type_filter.green_borderlands_map();
    map_type_filter.red_home_map = proto_map_type_filter.red_home_map();
    map_type_filter.red_borderlands_map = proto_map_type_filter.red_borderlands_map();
    map_type_filter.fortunes_vale_map = proto_map_type_filter.fortunes_vale_map();
    map_type_filter.jump_puzzle_map = proto_map_type_filter.jump_puzzle_map();
    map_type_filter.obsidian_sanctum_map = proto_map_type_filter.obsidian_sanctum_map();
    map_type_filter.edge_of_the_mists_map = proto_map_type_filter.edge_of_the_mists_map();
    map_type_filter.public_mini_map = proto_map_type_filter.public_mini_map();
    map_type_filter.wvw_lounge_map = proto_map_type_filter.wvw_lounge_map();
    return map_type_filter;
}
