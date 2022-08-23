#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;
class TrailData {
public:    
    string trail_data;
};

TrailData parse_trail_data(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
