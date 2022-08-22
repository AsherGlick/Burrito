#include "mount_filter_gen.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

MountFilter parse_mount_filter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
    MountFilter mount_filter;
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");	
	mount_filter.raptor = false;	
	mount_filter.springer = false;	
	mount_filter.skimmer = false;	
	mount_filter.jackal = false;	
	mount_filter.griffon = false;	
	mount_filter.roller_beetle = false;	
	mount_filter.warclaw = false;	
	mount_filter.skyscale = false;	
	mount_filter.skiff = false;	
	mount_filter.seige_turtle = false;
   
    for (string flag_value : flag_values) {
		if (flag_value == "Raptor") {
			mount_filter.raptor = true; 
		}
		else if (flag_value == "Springer") {
			mount_filter.springer = true; 
		}
		else if (flag_value == "Skimmer") {
			mount_filter.skimmer = true; 
		}
		else if (flag_value == "Jackal") {
			mount_filter.jackal = true; 
		}
		else if (flag_value == "Griffon") {
			mount_filter.griffon = true; 
		}
		else if (flag_value == "RollerBeetle") {
			mount_filter.roller_beetle = true; 
		}
		else if (flag_value == "Warclaw") {
			mount_filter.warclaw = true; 
		}
		else if (flag_value == "Skyscale") {
			mount_filter.skyscale = true; 
		}
		else if (flag_value == "Skiff") {
			mount_filter.skiff = true; 
		}
		else if (flag_value == "SeigeTurtle") {
			mount_filter.seige_turtle = true; 
		}
		else {
			errors->push_back(new XMLAttributeValueError("Found a value that was not in the class", input));
        	continue;
        }
    }
    return mount_filter;
	
}