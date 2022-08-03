#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

class EulerRotation {
 public:
    float x;
    float y;
    float z;
};

EulerRotation parse_EulerAngle(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
