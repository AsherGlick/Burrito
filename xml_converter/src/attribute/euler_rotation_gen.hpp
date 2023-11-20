#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/xml_reader_state.hpp"

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
    XMLReaderState* state,
    EulerRotation* value,
    bool* is_set);

std::string euler_rotation_to_xml_attribute(const std::string& attribute_name, const EulerRotation* value);

void proto_to_euler_rotation(waypoint::EulerRotation input, EulerRotation* value, bool* is_set);

void euler_rotation_to_proto(EulerRotation value, std::function<void(waypoint::EulerRotation*)> setter);
