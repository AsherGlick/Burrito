#include "map_type_filter_gen.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

MapTypeFilter parse_map_type_filter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
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

string stringify_map_type_filter(MapTypeFilter attribute_value){
    string output = "";
    if (attribute_value.unknown_map == true){
        output = output + "unknown";
    }
    if (attribute_value.redirect_map == true){
        output = output + "redirect";
    }
    if (attribute_value.character_create_map == true){
        output = output + "charactercreate";
    }
    if (attribute_value.pvp_map == true){
        output = output + "pvp";
    }
    if (attribute_value.gvg_map == true){
        output = output + "gvg";
    }
    if (attribute_value.instance_map == true){
        output = output + "instance";
    }
    if (attribute_value.public_map == true){
        output = output + "public";
    }
    if (attribute_value.tournament_map == true){
        output = output + "tournament";
    }
    if (attribute_value.tutorial_map == true){
        output = output + "tutorial";
    }
    if (attribute_value.user_tournament_map == true){
        output = output + "usertournament";
    }
    if (attribute_value.center_map == true){
        output = output + "center";
    }
    if (attribute_value.eternal_battlegrounds_map == true){
        output = output + "eternalbattlegrounds";
    }
    if (attribute_value.bluehome_map == true){
        output = output + "bluehome";
    }
    if (attribute_value.blue_borderlands_map == true){
        output = output + "blueborderlands";
    }
    if (attribute_value.green_home_map == true){
        output = output + "greenhome";
    }
    if (attribute_value.green_borderlands_map == true){
        output = output + "greenborderlands";
    }
    if (attribute_value.red_home_map == true){
        output = output + "redhome";
    }
    if (attribute_value.red_borderlands_map == true){
        output = output + "redborderlands";
    }
    if (attribute_value.fortunes_vale_map == true){
        output = output + "fortunesvale";
    }
    if (attribute_value.jump_puzzle_map == true){
        output = output + "jumppuzzle";
    }
    if (attribute_value.obsidian_sanctum_map == true){
        output = output + "obsidiansanctum";
    }
    if (attribute_value.edge_of_the_mists_map == true){
        output = output + "edgeofthemists";
    }
    if (attribute_value.public_mini_map == true){
        output = output + "publicmini";
    }
    if (attribute_value.wvw_lounge_map == true){
        output = output + "wvwlounge";
    }
    return output;
}