#include "euler_rotation_gen.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "guildpoint.pb.h"

using namespace std;

void xml_attribute_to_euler_rotation(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>*,
    XMLReaderState*,
    EulerRotation* value,
    bool* is_set) {
    EulerRotation euler_rotation;
    vector<string> compound_values;
    string attributename;
    euler_rotation.x_rotation = 0;
    euler_rotation.y_rotation = 0;
    euler_rotation.z_rotation = 0;
    attributename = get_attribute_name(input);
    compound_values = split(get_attribute_value(input), ",");
    if (compound_values.size() == 3) {
        euler_rotation.x_rotation = std::stof(compound_values[0]);
        euler_rotation.y_rotation = std::stof(compound_values[1]);
        euler_rotation.z_rotation = std::stof(compound_values[2]);
    }
    *value = euler_rotation;
    *is_set = true;
}
string euler_rotation_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState*,
    const EulerRotation* value) {
    string output;
    output = to_string(value->x_rotation);
    output = output + "," + to_string(value->y_rotation);
    output = output + "," + to_string(value->z_rotation);
    return " " + attribute_name + "=\"" + output + "\"";
}

void proto_to_euler_rotation(
    guildpoint::EulerRotation input,
    ProtoReaderState*,
    EulerRotation* value,
    bool* is_set) {
    EulerRotation euler_rotation;
    euler_rotation.x_rotation = input.x();
    euler_rotation.y_rotation = input.y();
    euler_rotation.z_rotation = input.z();
    *value = euler_rotation;
    *is_set = true;
}

void euler_rotation_to_proto(
    EulerRotation value,
    ProtoWriterState*,
    std::function<void(guildpoint::EulerRotation*)> setter) {
    guildpoint::EulerRotation* proto_euler_rotation = new guildpoint::EulerRotation();
    proto_euler_rotation->set_x(value.x_rotation);
    proto_euler_rotation->set_y(value.y_rotation);
    proto_euler_rotation->set_z(value.z_rotation);
    setter(proto_euler_rotation);
}
