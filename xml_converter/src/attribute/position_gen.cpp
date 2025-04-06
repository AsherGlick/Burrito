#include "position_gen.hpp"

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
// Reads a Position from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Position::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>*,
    XMLReaderState*,
    Position* value,
    bool* is_set
) {
    Position position;
    vector<string> compound_values;
    string attributename;
    position.x_position = 0;
    position.y_position = 0;
    position.z_position = 0;
    attributename = get_attribute_name(input);
    compound_values = split(get_attribute_value(input), ",");
    if (compound_values.size() == 3) {
        position.x_position = std::stof(compound_values[0]);
        position.y_position = std::stof(compound_values[1]);
        position.z_position = std::stof(compound_values[2]);
    }
    *value = position;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads a Position from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Position::from_proto_field(
    guildpoint::Position input,
    ProtoReaderState*,
    Position* value,
    bool* is_set
) {
    Position position;
    position.x_position = input.x();
    position.y_position = input.y();
    position.z_position = input.z();
    *value = position;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes a Position to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Position::to_proto_field(
    Position value,
    ProtoWriterState*,
    std::function<void(guildpoint::Position*)> setter
) {
    guildpoint::Position* proto_position = new guildpoint::Position();
    proto_position->set_x(value.x_position);
    proto_position->set_y(value.y_position);
    proto_position->set_z(value.z_position);
    setter(proto_position);
}
