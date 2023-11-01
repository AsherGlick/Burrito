#include "position_gen.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "waypoint.pb.h"

using namespace std;

void xml_attribute_to_position(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    Position* value,
    bool* is_set) {
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

waypoint::Position* to_proto_position(Position attribute_value) {
    waypoint::Position* proto_position = new waypoint::Position();
    proto_position->set_x(attribute_value.x_position);
    proto_position->set_y(attribute_value.y_position);
    proto_position->set_z(attribute_value.z_position);
    return proto_position;
}

Position from_proto_position(waypoint::Position proto_position) {
    Position position;
    position.x_position = proto_position.x();
    position.y_position = proto_position.y();
    position.z_position = proto_position.z();
    return position;
}
