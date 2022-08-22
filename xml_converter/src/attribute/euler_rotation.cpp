#include "euler_rotation.hpp"

#include <string>
#include <vector>
#include <typeinfo>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

EulerRotation parse_euler_rotation(rapidxml::xml_attribute<>* input, vector<XMLError*> *){
    EulerRotation euler_rotation;
    vector<string> compound_values;
    string attributename;
    euler_rotation.x_rotation = 0;
    euler_rotation.y_rotation = 0;
    euler_rotation.z_rotation = 0; 
    attributename = get_attribute_name(input); 
    compound_values = split(get_attribute_value(input), ",");
        
    if (typeid(compound_values) == typeid(std::string)) {
        if (attributename == "rotatex") {	
            euler_rotation.x_rotation = std::stof(get_attribute_value(input)); 
    	}
        else if (attributename == "rotatey") {
            euler_rotation.y_rotation = std::stof(get_attribute_value(input)); 
        }
        else if (attributename == "rotatez") {
            euler_rotation.z_rotation = std::stof(get_attribute_value(input)); 
        }
    else {    
        euler_rotation.x_rotation = std::stof(compound_values[0]);    
        euler_rotation.y_rotation = std::stof(compound_values[1]);    
        euler_rotation.z_rotation = std::stof(compound_values[2]); 
        }
    }

    return euler_rotation;
	
}