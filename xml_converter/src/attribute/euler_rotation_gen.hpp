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
class EulerRotation;
}

namespace Attribute::EulerRotation {

class EulerRotation {
 public:
    float x_rotation;
    float y_rotation;
    float z_rotation;

    virtual std::string classname() {
        return "EulerRotation";
    }
};
void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    EulerRotation* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const EulerRotation* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    guildpoint::EulerRotation input,
    ProtoReaderState* state,
    EulerRotation* value,
    bool* is_set
);

void to_proto_field(
    EulerRotation value,
    ProtoWriterState* state,
    std::function<void(guildpoint::EulerRotation*)> setter
);

}  // namespace Attribute::EulerRotation
