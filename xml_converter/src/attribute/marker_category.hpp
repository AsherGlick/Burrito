#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

class MarkerCategory {
 public:    
    string category;
};

MarkerCategory parse_marker_category(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);

string stringify_marker_category (MarkerCategory attribute_value); 