#include "map_type_filter_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "guildpoint.pb.h"

using namespace std;

void xml_attribute_to_map_type_filter(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    MapTypeFilter* value,
    bool* is_set) {
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
    *value = map_type_filter;
    *is_set = true;
}

string map_type_filter_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState*,
    const MapTypeFilter* value) {
    vector<string> flag_values;
    if (value->unknown_map == true) {
        flag_values.push_back("unknown");
    }
    if (value->redirect_map == true) {
        flag_values.push_back("redirect");
    }
    if (value->character_create_map == true) {
        flag_values.push_back("charactercreate");
    }
    if (value->pvp_map == true) {
        flag_values.push_back("pvp");
    }
    if (value->gvg_map == true) {
        flag_values.push_back("gvg");
    }
    if (value->instance_map == true) {
        flag_values.push_back("instance");
    }
    if (value->public_map == true) {
        flag_values.push_back("public");
    }
    if (value->tournament_map == true) {
        flag_values.push_back("tournament");
    }
    if (value->tutorial_map == true) {
        flag_values.push_back("tutorial");
    }
    if (value->user_tournament_map == true) {
        flag_values.push_back("usertournament");
    }
    if (value->center_map == true) {
        flag_values.push_back("center");
    }
    if (value->eternal_battlegrounds_map == true) {
        flag_values.push_back("eternalbattlegrounds");
    }
    if (value->bluehome_map == true) {
        flag_values.push_back("bluehome");
    }
    if (value->blue_borderlands_map == true) {
        flag_values.push_back("blueborderlands");
    }
    if (value->green_home_map == true) {
        flag_values.push_back("greenhome");
    }
    if (value->green_borderlands_map == true) {
        flag_values.push_back("greenborderlands");
    }
    if (value->red_home_map == true) {
        flag_values.push_back("redhome");
    }
    if (value->red_borderlands_map == true) {
        flag_values.push_back("redborderlands");
    }
    if (value->fortunes_vale_map == true) {
        flag_values.push_back("fortunesvale");
    }
    if (value->jump_puzzle_map == true) {
        flag_values.push_back("jumppuzzle");
    }
    if (value->obsidian_sanctum_map == true) {
        flag_values.push_back("obsidiansanctum");
    }
    if (value->edge_of_the_mists_map == true) {
        flag_values.push_back("edgeofthemists");
    }
    if (value->public_mini_map == true) {
        flag_values.push_back("publicmini");
    }
    if (value->wvw_lounge_map == true) {
        flag_values.push_back("wvwlounge");
    }
    string output = join(flag_values, ",");
    return " " + attribute_name + "=\"" + output + "\"";
}

void proto_to_map_type_filter(
    guildpoint::MapTypeFilter input,
    ProtoReaderState*,
    MapTypeFilter* value,
    bool* is_set) {
    MapTypeFilter map_type_filter;
    map_type_filter.unknown_map = input.unknown_map();
    map_type_filter.redirect_map = input.redirect_map();
    map_type_filter.character_create_map = input.character_create_map();
    map_type_filter.pvp_map = input.pvp_map();
    map_type_filter.gvg_map = input.gvg_map();
    map_type_filter.instance_map = input.instance_map();
    map_type_filter.public_map = input.public_map();
    map_type_filter.tournament_map = input.tournament_map();
    map_type_filter.tutorial_map = input.tutorial_map();
    map_type_filter.user_tournament_map = input.user_tournament_map();
    map_type_filter.center_map = input.center_map();
    map_type_filter.eternal_battlegrounds_map = input.eternal_battlegrounds_map();
    map_type_filter.bluehome_map = input.bluehome_map();
    map_type_filter.blue_borderlands_map = input.blue_borderlands_map();
    map_type_filter.green_home_map = input.green_home_map();
    map_type_filter.green_borderlands_map = input.green_borderlands_map();
    map_type_filter.red_home_map = input.red_home_map();
    map_type_filter.red_borderlands_map = input.red_borderlands_map();
    map_type_filter.fortunes_vale_map = input.fortunes_vale_map();
    map_type_filter.jump_puzzle_map = input.jump_puzzle_map();
    map_type_filter.obsidian_sanctum_map = input.obsidian_sanctum_map();
    map_type_filter.edge_of_the_mists_map = input.edge_of_the_mists_map();
    map_type_filter.public_mini_map = input.public_mini_map();
    map_type_filter.wvw_lounge_map = input.wvw_lounge_map();
    *value = map_type_filter;
    *is_set = true;
}

void map_type_filter_to_proto(
    MapTypeFilter value,
    ProtoWriterState*,
    std::function<void(guildpoint::MapTypeFilter*)> setter) {
    guildpoint::MapTypeFilter* proto_map_type_filter = new guildpoint::MapTypeFilter();
    bool should_write = false;
    proto_map_type_filter->set_unknown_map(value.unknown_map);
    should_write |= value.unknown_map;
    proto_map_type_filter->set_redirect_map(value.redirect_map);
    should_write |= value.redirect_map;
    proto_map_type_filter->set_character_create_map(value.character_create_map);
    should_write |= value.character_create_map;
    proto_map_type_filter->set_pvp_map(value.pvp_map);
    should_write |= value.pvp_map;
    proto_map_type_filter->set_gvg_map(value.gvg_map);
    should_write |= value.gvg_map;
    proto_map_type_filter->set_instance_map(value.instance_map);
    should_write |= value.instance_map;
    proto_map_type_filter->set_public_map(value.public_map);
    should_write |= value.public_map;
    proto_map_type_filter->set_tournament_map(value.tournament_map);
    should_write |= value.tournament_map;
    proto_map_type_filter->set_tutorial_map(value.tutorial_map);
    should_write |= value.tutorial_map;
    proto_map_type_filter->set_user_tournament_map(value.user_tournament_map);
    should_write |= value.user_tournament_map;
    proto_map_type_filter->set_center_map(value.center_map);
    should_write |= value.center_map;
    proto_map_type_filter->set_eternal_battlegrounds_map(value.eternal_battlegrounds_map);
    should_write |= value.eternal_battlegrounds_map;
    proto_map_type_filter->set_bluehome_map(value.bluehome_map);
    should_write |= value.bluehome_map;
    proto_map_type_filter->set_blue_borderlands_map(value.blue_borderlands_map);
    should_write |= value.blue_borderlands_map;
    proto_map_type_filter->set_green_home_map(value.green_home_map);
    should_write |= value.green_home_map;
    proto_map_type_filter->set_green_borderlands_map(value.green_borderlands_map);
    should_write |= value.green_borderlands_map;
    proto_map_type_filter->set_red_home_map(value.red_home_map);
    should_write |= value.red_home_map;
    proto_map_type_filter->set_red_borderlands_map(value.red_borderlands_map);
    should_write |= value.red_borderlands_map;
    proto_map_type_filter->set_fortunes_vale_map(value.fortunes_vale_map);
    should_write |= value.fortunes_vale_map;
    proto_map_type_filter->set_jump_puzzle_map(value.jump_puzzle_map);
    should_write |= value.jump_puzzle_map;
    proto_map_type_filter->set_obsidian_sanctum_map(value.obsidian_sanctum_map);
    should_write |= value.obsidian_sanctum_map;
    proto_map_type_filter->set_edge_of_the_mists_map(value.edge_of_the_mists_map);
    should_write |= value.edge_of_the_mists_map;
    proto_map_type_filter->set_public_mini_map(value.public_mini_map);
    should_write |= value.public_mini_map;
    proto_map_type_filter->set_wvw_lounge_map(value.wvw_lounge_map);
    should_write |= value.wvw_lounge_map;
    if (should_write) {
        setter(proto_map_type_filter);
    }
}
