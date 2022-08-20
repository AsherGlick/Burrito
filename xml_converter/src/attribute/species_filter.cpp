#include "species_filter.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

SpeciesFilter parse_species_filter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
    SpeciesFilter species_filter;
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");
   
    for (string flag_value : flag_values) {
		if (flag_value == "asura") {
			species_filter.asura = true; 
		}
		else if (flag_value == "charr") {
			species_filter.charr = true; 
		}
		else if (flag_value == "human") {
			species_filter.human = true; 
		}
		else if (flag_value == "norn") {
			species_filter.norn = true; 
		}
		else if (flag_value == "sylvari") {
			species_filter.sylvari = true; 
		}
		else {errors->push_back(new XMLAttributeValueError("Found a value that was not in the class", input));
        	continue;
        }
    }
    return species_filter;
	
}