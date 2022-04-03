#include "euler_angle.hpp"

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;


EulerAngle parse_EulerAngle(rapidxml::xml_attribute<>* input, vector<string> *errors) {
    vector<string> components = split(string(input->value()), ",");

    EulerAngle euler_angle;
    if (components.size() == 3) {
        euler_angle.x = stof(components[0].c_str());
        euler_angle.y = stof(components[1].c_str());
        euler_angle.z = stof(components[2].c_str());
    }
    else {
        errors->push_back("invlaid 'x,y,z' rotation " + string(input->value()) + " " + to_string(components.size()));
    }

    return euler_angle;
}
