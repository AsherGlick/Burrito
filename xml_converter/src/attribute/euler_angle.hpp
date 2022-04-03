#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

class EulerAngle {
 public:
    float x;
    float y;
    float z;
};

EulerAngle parse_EulerAngle(rapidxml::xml_attribute<>* input, vector<string> *errors);
