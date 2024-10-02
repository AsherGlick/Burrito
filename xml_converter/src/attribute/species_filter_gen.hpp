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
void xml_attribute_to_species_filter(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    SpeciesFilter* value,
    bool* is_set);

std::string species_filter_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const SpeciesFilter* value);

void proto_to_species_filter(
    guildpoint::SpeciesFilter input,
    ProtoReaderState* state,
    SpeciesFilter* value,
    bool* is_set);

void species_filter_to_proto(
    SpeciesFilter value,
    ProtoWriterState* state,
    std::function<void(guildpoint::SpeciesFilter*)> setter);
