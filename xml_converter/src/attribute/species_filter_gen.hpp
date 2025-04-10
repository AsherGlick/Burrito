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
class SpeciesFilter;
}

namespace Attribute::SpeciesFilter {

class SpeciesFilter {
 public:
    bool asura;
    bool charr;
    bool human;
    bool norn;
    bool sylvari;

    virtual std::string classname() {
        return "SpeciesFilter";
    }
};
void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    SpeciesFilter* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const SpeciesFilter* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    guildpoint::SpeciesFilter input,
    ProtoReaderState* state,
    SpeciesFilter* value,
    bool* is_set
);

void to_proto_field(
    SpeciesFilter value,
    ProtoWriterState* state,
    std::function<void(guildpoint::SpeciesFilter*)> setter
);

}  // namespace Attribute::SpeciesFilter
