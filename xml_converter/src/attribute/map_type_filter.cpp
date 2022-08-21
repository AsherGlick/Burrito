#include "map_type_filter.hpp"

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
		if (flag_value == "Unknown") {
			map_type_filter.unknown_map = true; 
		}
		else if (flag_value == "redirect") {
			map_type_filter.redirect_map = true; 
		}
		else if (flag_value == "charactercreate") {
			map_type_filter.character_create_map = true; 
		}
		else if (flag_value == "pvp") {
			map_type_filter.pvp_map = true; 
		}
		else if (flag_value == "gvg") {
			map_type_filter.gvg_map = true; 
		}
		else if (flag_value == "instance") {
			map_type_filter.instance_map = true; 
		}
		else if (flag_value == "public") {
			map_type_filter.public_map = true; 
		}
		else if (flag_value == "tournament") {
			map_type_filter.tournament_map = true; 
		}
		else if (flag_value == "tutorial") {
			map_type_filter.tutorial_map = true; 
		}
		else if (flag_value == "usertournament") {
			map_type_filter.user_tournament_map = true; 
		}
		else if (flag_value == "center") {
			map_type_filter.center_map = true; 
		}
		else if (flag_value == "eternalbattlegrounds") {
			map_type_filter.eternal_battlegrounds_map = true; 
		}
		else if (flag_value == "bluehome") {
			map_type_filter.bluehome_map = true; 
		}
		else if (flag_value == "blueborderlands") {
			map_type_filter.blue_borderlands_map = true; 
		}
		else if (flag_value == "greenhome") {
			map_type_filter.green_home_map = true; 
		}
		else if (flag_value == "greenborderlands") {
			map_type_filter.green_borderlands_map = true; 
		}
		else if (flag_value == "redhome") {
			map_type_filter.red_home_map = true; 
		}
		else if (flag_value == "redborderlands") {
			map_type_filter.red_borderlands_map = true; 
		}
		else if (flag_value == "fortunesvale") {
			map_type_filter.fortunes_vale_map = true; 
		}
		else if (flag_value == "jumppuzzle") {
			map_type_filter.jump_puzzle_map = true; 
		}
		else if (flag_value == "obsidiansanctum") {
			map_type_filter.obsidian_sanctum_map = true; 
		}
		else if (flag_value == "edgeofthemists") {
			map_type_filter.edge_of_the_mists_map = true; 
		}
		else if (flag_value == "publicmini") {
			map_type_filter.public_mini_map = true; 
		}
		else if (flag_value == "wvwlounge") {
			map_type_filter.wvw_lounge_map = true; 
		}
		else {
			errors->push_back(new XMLAttributeValueError("Found a value that was not in the class", input));
        	continue;
        }
    }
    return map_type_filter;
	
}