#pragma once

#include <string>
#include <vector>
#include <functional>

#include "../rapidxml-1.13/rapidxml.hpp"
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
    SpeciesFilter* value,
    bool* is_set);
std::string species_filter_to_xml_attribute(const std::string& attribute_name, const SpeciesFilter* value);

SpeciesFilter from_proto_species_filter(waypoint::SpeciesFilter proto_species_filter);

void species_filter_to_proto(SpeciesFilter value, std::function<void(waypoint::SpeciesFilter*)> setter);
