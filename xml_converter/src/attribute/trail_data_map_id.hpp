#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;
class TrailDataMapId {
public:    
    int trail_data_map_id;
};

TrailDataMapId parse_trail_data_map_id(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
