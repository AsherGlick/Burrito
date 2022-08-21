#include "festival_filter.hpp"

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
		if (flag_value == "DragonBash") {
			festival_filter.dragonbash = true; 
		}
		else if (flag_value == "FestivalOfTheFourWinds") {
			festival_filter.festival_of_the_four_winds = true; 
		}
		else if (flag_value == "Halloween") {
			festival_filter.halloween = true; 
		}
		else if (flag_value == "LunarNewYear") {
			festival_filter.lunar_new_year = true; 
		}
		else if (flag_value == "SuperAdventureFestival") {
			festival_filter.super_adventure_festival = true; 
		}
		else if (flag_value == "SuperAdventureBox") {
			festival_filter.super_adventure_festival = true; 
		}
		else if (flag_value == "Wintersday") {
			festival_filter.wintersday = true; 
		}
		else if (flag_value == "None") {
			festival_filter.none = true; 
		}
		else {
			errors->push_back(new XMLAttributeValueError("Found a value that was not in the class", input));
        	continue;
        }
    }
    return festival_filter;
	
}