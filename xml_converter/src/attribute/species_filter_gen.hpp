#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

class XMLError;

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
SpeciesFilter parse_species_filter(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);
std::string stringify_species_filter(SpeciesFilter attribute_value);
waypoint::SpeciesFilter* to_proto_species_filter(SpeciesFilter attribute_value);
SpeciesFilter from_proto_species_filter(waypoint::SpeciesFilter proto_species_filter);
