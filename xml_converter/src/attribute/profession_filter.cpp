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
    	string normalized_flag_value = normalize(flag_value);
		if (normalized_flag_value == "guardian") {
			profession_filter.guardian = true; 
		}
		else if (normalized_flag_value == "warrior") {
			profession_filter.warrior = true; 
		}
		else if (normalized_flag_value == "engineer") {
			profession_filter.engineer = true; 
		}
		else if (normalized_flag_value == "ranger") {
			profession_filter.ranger = true; 
		}
		else if (normalized_flag_value == "thief") {
			profession_filter.thief = true; 
		}
		else if (normalized_flag_value == "elementalist") {
			profession_filter.elementalist = true; 
		}
		else if (normalized_flag_value == "mesmer") {
			profession_filter.mesmer = true; 
		}
		else if (normalized_flag_value == "necromancer") {
			profession_filter.necromancer = true; 
		}
		else if (normalized_flag_value == "revenant") {
			profession_filter.revenant = true; 
		}
		else {
			errors->push_back(new XMLAttributeValueError("Invalid Filter for ProfessionFilter. Found " + flag_value, input));
        	continue;
        }
    }
    return profession_filter;
	
}