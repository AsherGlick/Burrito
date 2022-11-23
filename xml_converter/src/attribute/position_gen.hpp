#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

class XMLError;

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
std::string stringify_position(Position attribute_value);
// waypoint::Position* to_proto_position(Position attribute_value);
