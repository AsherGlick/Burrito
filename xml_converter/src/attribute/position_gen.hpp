#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/xml_parse_state.hpp"

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
void xml_attribute_to_position(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLParseState* state,
    Position* value,
    bool* is_set);

std::string position_to_xml_attribute(const std::string& attribute_name, const Position* value);

void proto_to_position(waypoint::Position input, Position* value, bool* is_set);

void position_to_proto(Position value, std::function<void(waypoint::Position*)> setter);
