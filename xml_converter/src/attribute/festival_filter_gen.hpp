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
class FestivalFilter;
}

namespace Attribute::FestivalFilter {

class FestivalFilter {
 public:
    bool dragonbash;
    bool festival_of_the_four_winds;
    bool halloween;
    bool lunar_new_year;
    bool none;
    bool super_adventure_festival;
    bool wintersday;

    virtual std::string classname() {
        return "FestivalFilter";
    }
};
void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    FestivalFilter* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const FestivalFilter* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    guildpoint::FestivalFilter input,
    ProtoReaderState* state,
    FestivalFilter* value,
    bool* is_set
);

void to_proto_field(
    FestivalFilter value,
    ProtoWriterState* state,
    std::function<void(guildpoint::FestivalFilter*)> setter
);

}  // namespace Attribute::FestivalFilter
