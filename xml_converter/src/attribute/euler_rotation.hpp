#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;
class EulerRotation {
 public:
    float x_rotation;
    float y_rotation;
    float z_rotation;

    virtual string classname() { return "EulerRotation"; };
};
EulerRotation parse_euler_rotation(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
 