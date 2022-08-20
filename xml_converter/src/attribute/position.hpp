#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;
class Position {
 public:
    float x_position;
    float y_position;
    float z_position;

    virtual string classname() { return "Position"; };
};
Position parse_position(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);