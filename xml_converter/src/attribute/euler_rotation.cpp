#include "euler_rotation.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

EulerRotation parse_euler_rotation(rapidxml::xml_attribute<>* input, vector<XMLError*> *){
    EulerRotation euler_rotation;
    vector<string> compound_values;
    string attributename; 
    attributename = get_attribute_name(input);
    if (attributename == "rotatex") {	
        euler_rotation.x_rotation = std::stof(get_attribute_value(input)); 
	}
    else if (attributename == "rotatey") {
        euler_rotation.y_rotation = std::stof(get_attribute_value(input)); 
    }
    else if (attributename == "rotatez") {
        euler_rotation.z_rotation = std::stof(get_attribute_value(input)); 
    }

    return euler_rotation;
	
} 