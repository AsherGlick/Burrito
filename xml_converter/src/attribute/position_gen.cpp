#include "position_gen.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

Position parse_position(rapidxml::xml_attribute<>* input, vector<XMLError*> *){
    Position position;
    vector<string> compound_values;
    string attributename;
    position.x_position = 0;
    position.y_position = 0;
    position.z_position = 0; 
    attributename = get_attribute_name(input); 
    compound_values = split(get_attribute_value(input), ",");
    if (compound_values.size() == 3){    
        position.x_position = std::stof(compound_values[0]);    
        position.y_position = std::stof(compound_values[1]);    
        position.z_position = std::stof(compound_values[2]); 
    }
    return position;
}