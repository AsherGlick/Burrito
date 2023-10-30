#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
class XMLError;
namespace waypoint {
class Position;
}

class Position {
 public:
    float x_position;
    float y_position;
    float z_position;

    virtual std::string classname() {
        return "Position";
    }
};
Position parse_position(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);
std::string position_to_xml_attribute(const std::string& attribute_name, const Position* value);
waypoint::Position* to_proto_position(Position attribute_value);
Position from_proto_position(waypoint::Position proto_position);
