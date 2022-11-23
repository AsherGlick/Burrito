#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

class XMLError;

class EulerRotation {
 public:
    float x_rotation;
    float y_rotation;
    float z_rotation;

    virtual std::string classname() {
        return "EulerRotation";
    }
};
EulerRotation parse_euler_rotation(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);
std::string stringify_euler_rotation(EulerRotation attribute_value);
// waypoint::EulerRotation* to_proto_euler_rotation(EulerRotation attribute_value);
