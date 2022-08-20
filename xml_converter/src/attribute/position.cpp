#include "position.hpp"

#include <string>
#include <vector>
#include <typeinfo>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

Position parse_position(rapidxml::xml_attribute<>* input, vector<XMLError*> *){
    Position position;
    vector<string> compound_values;
    string attributename; 
    attributename = get_attribute_name(input); 
    compound_values = split(get_attribute_value(input), ",");
    
    if (typeid(compound_values) == typeid(std::string)) {
        if (attributename == "xpos") {	
            position.x_position = std::stof(get_attribute_value(input)); 
    	}
        else if (attributename == "positionx") {
            position.x_position = std::stof(get_attribute_value(input)); 
        }
        else if (attributename == "ypos") {
            position.y_position = std::stof(get_attribute_value(input)); 
        }
        else if (attributename == "positiony") {
            position.y_position = std::stof(get_attribute_value(input)); 
        }
        else if (attributename == "zpos") {
            position.z_position = std::stof(get_attribute_value(input)); 
        }
        else if (attributename == "positionz") {
            position.z_position = std::stof(get_attribute_value(input)); 
        }
    else {    
        position.x_position = std::stof(compound_values[0]);    
        position.y_position = std::stof(compound_values[1]);    
        position.z_position = std::stof(compound_values[2]); 
        }
    }

    return position;
	
} 