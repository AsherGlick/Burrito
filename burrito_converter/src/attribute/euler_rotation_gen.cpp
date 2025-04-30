#include "euler_rotation_gen.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "guildpoint.pb.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads a EulerRotation from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::EulerRotation::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>*,
    XMLReaderState*,
    EulerRotation* value,
    bool* is_set
) {
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

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Writes a EulerRotation to an xml attribute using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::EulerRotation::to_xml_attribute(
    XMLWriterState*,
    const EulerRotation* value,
    std::function<void(std::string)> setter
) {
    string output;
    output = to_string(value->x_rotation);
    output = output + "," + to_string(value->y_rotation);
    output = output + "," + to_string(value->z_rotation);
    setter(output);
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads a EulerRotation from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::EulerRotation::from_proto_field(
    guildpoint::EulerRotation input,
    ProtoReaderState*,
    EulerRotation* value,
    bool* is_set
) {
    EulerRotation euler_rotation;
    euler_rotation.x_rotation = input.x();
    euler_rotation.y_rotation = input.y();
    euler_rotation.z_rotation = input.z();
    *value = euler_rotation;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes a EulerRotation to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::EulerRotation::to_proto_field(
    EulerRotation value,
    ProtoWriterState*,
    std::function<void(guildpoint::EulerRotation*)> setter
) {
    guildpoint::EulerRotation* proto_euler_rotation = new guildpoint::EulerRotation();
    proto_euler_rotation->set_x(value.x_rotation);
    proto_euler_rotation->set_y(value.y_rotation);
    proto_euler_rotation->set_z(value.z_rotation);
    setter(proto_euler_rotation);
}
