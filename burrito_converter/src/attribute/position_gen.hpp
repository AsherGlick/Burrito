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

namespace Attribute::Position {

class Position {
 public:
    float x_position;
    float y_position;
    float z_position;

    virtual std::string classname() {
        return "Position";
    }
};
void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    Position* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const Position* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    guildpoint::Position input,
    ProtoReaderState* state,
    Position* value,
    bool* is_set
);

void to_proto_field(
    Position value,
    ProtoWriterState* state,
    std::function<void(guildpoint::Position*)> setter
);

}  // namespace Attribute::Position
