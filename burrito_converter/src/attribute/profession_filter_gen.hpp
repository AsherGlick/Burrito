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
class ProfessionFilter;
}

namespace Attribute::ProfessionFilter {

class ProfessionFilter {
 public:
    bool elementalist;
    bool engineer;
    bool guardian;
    bool mesmer;
    bool necromancer;
    bool ranger;
    bool revenant;
    bool thief;
    bool warrior;

    virtual std::string classname() {
        return "ProfessionFilter";
    }
};
void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    ProfessionFilter* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const ProfessionFilter* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    guildpoint::ProfessionFilter input,
    ProtoReaderState* state,
    ProfessionFilter* value,
    bool* is_set
);

void to_proto_field(
    ProfessionFilter value,
    ProtoWriterState* state,
    std::function<void(guildpoint::ProfessionFilter*)> setter
);

}  // namespace Attribute::ProfessionFilter
