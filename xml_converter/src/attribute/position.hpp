#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;


class Position {
 public:
    float x;
    float y;
    float z;
};

Position parse_Position(rapidxml::xml_attribute<>* input, vector<string> *errors);
