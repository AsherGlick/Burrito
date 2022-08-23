#include "festival_filter_gen.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

FestivalFilter parse_festival_filter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
    FestivalFilter festival_filter;
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");	
	festival_filter.dragonbash = false;	
	festival_filter.festival_of_the_four_winds = false;	
	festival_filter.halloween = false;	
	festival_filter.lunar_new_year = false;	
	festival_filter.super_adventure_festival = false;	
	festival_filter.wintersday = false;	
	festival_filter.none = false;
   
    for (string flag_value : flag_values) {
    	string normalized_flag_value = normalize(flag_value);
		if (normalized_flag_value == "dragonbash") {
			festival_filter.dragonbash = true; 
		}
		else if (normalized_flag_value == "festivalofthefourwinds") {
			festival_filter.festival_of_the_four_winds = true; 
		}
		else if (normalized_flag_value == "halloween") {
			festival_filter.halloween = true; 
		}
		else if (normalized_flag_value == "lunarnewyear") {
			festival_filter.lunar_new_year = true; 
		}
		else if (normalized_flag_value == "superadventurefestival") {
			festival_filter.super_adventure_festival = true; 
		}
		else if (normalized_flag_value == "superadventurebox") {
			festival_filter.super_adventure_festival = true; 
		}
		else if (normalized_flag_value == "wintersday") {
			festival_filter.wintersday = true; 
		}
		else if (normalized_flag_value == "none") {
			festival_filter.none = true; 
		}
		else {
			errors->push_back(new XMLAttributeValueError("Invalid Filter for FestivalFilter. Found " + flag_value, input));
        	continue;
        }
    }
    return festival_filter;
	
}