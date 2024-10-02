#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/proto_reader_state.hpp"
#include "../state_structs/proto_writer_state.hpp"
#include "../state_structs/xml_reader_state.hpp"
#include "../state_structs/xml_writer_state.hpp"

class XMLError;
namespace guildpoint {
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
    XMLReaderState* state,
    Position* value,
    bool* is_set);

std::string position_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const Position* value);

void proto_to_position(
    guildpoint::Position input,
    ProtoReaderState* state,
    Position* value,
    bool* is_set);

void position_to_proto(
    Position value,
    ProtoWriterState* state,
    std::function<void(guildpoint::Position*)> setter);
