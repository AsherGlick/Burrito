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
class MountFilter;
}

namespace Attribute::MountFilter {

class MountFilter {
 public:
    bool griffon;
    bool jackal;
    bool raptor;
    bool roller_beetle;
    bool seige_turtle;
    bool skiff;
    bool skimmer;
    bool skyscale;
    bool springer;
    bool warclaw;

    virtual std::string classname() {
        return "MountFilter";
    }
};
void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    MountFilter* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const MountFilter* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    guildpoint::MountFilter input,
    ProtoReaderState* state,
    MountFilter* value,
    bool* is_set
);

void to_proto_field(
    MountFilter value,
    ProtoWriterState* state,
    std::function<void(guildpoint::MountFilter*)> setter
);

}  // namespace Attribute::MountFilter
