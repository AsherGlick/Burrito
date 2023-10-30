#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
class XMLError;
namespace waypoint {
class EulerRotation;
}

class EulerRotation {
 public:
    float x_rotation;
    float y_rotation;
    float z_rotation;

    virtual std::string classname() {
        return "EulerRotation";
    }
};
void xml_attribute_to_euler_rotation(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    EulerRotation* value,
    bool* is_set);
std::string euler_rotation_to_xml_attribute(const std::string& attribute_name, const EulerRotation* value);
waypoint::EulerRotation* to_proto_euler_rotation(EulerRotation attribute_value);
EulerRotation from_proto_euler_rotation(waypoint::EulerRotation proto_euler_rotation);
