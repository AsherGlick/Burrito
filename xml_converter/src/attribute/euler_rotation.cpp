#include "euler_rotation.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;


EulerRotation parse_EulerAngle(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors) {
    vector<string> components = split(get_attribute_value(input), ",");

    EulerRotation euler_angle;
    if (components.size() == 3) {
        euler_angle.x = stof(components[0].c_str());
        euler_angle.y = stof(components[1].c_str());
        euler_angle.z = stof(components[2].c_str());
    }
    else {
        errors->push_back(new XMLAttributeValueError("Invlaid 'x,y,z' rotation ", input));
    }

    return euler_angle;
}
