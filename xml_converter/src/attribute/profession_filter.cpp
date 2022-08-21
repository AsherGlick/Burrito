#include "profession_filter.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

ProfessionFilter parse_profession_filter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
    ProfessionFilter profession_filter;
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");	
	profession_filter.guardian = false;	
	profession_filter.warrior = false;	
	profession_filter.engineer = false;	
	profession_filter.ranger = false;	
	profession_filter.thief = false;	
	profession_filter.elementalist = false;	
	profession_filter.mesmer = false;	
	profession_filter.necromancer = false;	
	profession_filter.revenant = false;
   
    for (string flag_value : flag_values) {
		if (flag_value == "Guardian") {
			profession_filter.guardian = true; 
		}
		else if (flag_value == "Warrior") {
			profession_filter.warrior = true; 
		}
		else if (flag_value == "Engineer") {
			profession_filter.engineer = true; 
		}
		else if (flag_value == "Ranger") {
			profession_filter.ranger = true; 
		}
		else if (flag_value == "Thief") {
			profession_filter.thief = true; 
		}
		else if (flag_value == "Elementalist") {
			profession_filter.elementalist = true; 
		}
		else if (flag_value == "Mesmer") {
			profession_filter.mesmer = true; 
		}
		else if (flag_value == "Necromancer") {
			profession_filter.necromancer = true; 
		}
		else if (flag_value == "Revenant") {
			profession_filter.revenant = true; 
		}
		else {
			errors->push_back(new XMLAttributeValueError("Found a value that was not in the class", input));
        	continue;
        }
    }
    return profession_filter;
	
}