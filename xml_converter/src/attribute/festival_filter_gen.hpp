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
void xml_attribute_to_festival_filter(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    FestivalFilter* value,
    bool* is_set);

std::string festival_filter_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const FestivalFilter* value);

void proto_to_festival_filter(
    guildpoint::FestivalFilter input,
    ProtoReaderState* state,
    FestivalFilter* value,
    bool* is_set);

void festival_filter_to_proto(
    FestivalFilter value,
    ProtoWriterState* state,
    std::function<void(guildpoint::FestivalFilter*)> setter);
