#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/xml_parse_state.hpp"

class XMLError;
namespace waypoint {
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
    XMLParseState* state,
    SpeciesFilter* value,
    bool* is_set);

std::string species_filter_to_xml_attribute(const std::string& attribute_name, const SpeciesFilter* value);

void proto_to_species_filter(waypoint::SpeciesFilter input, SpeciesFilter* value, bool* is_set);

void species_filter_to_proto(SpeciesFilter value, std::function<void(waypoint::SpeciesFilter*)> setter);
