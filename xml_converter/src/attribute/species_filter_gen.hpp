#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;
class SpeciesFilter {
 public:
    bool asura;
    bool charr;
    bool human;
    bool norn;
    bool sylvari;

    virtual string classname() { return "SpeciesFilter"; };
};
SpeciesFilter parse_species_filter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
string stringify_species_filter(SpeciesFilter attribute_value);