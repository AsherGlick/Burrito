#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "filter.hpp"

using namespace std;

#define FILTER_ITEM(...) CLASS_FILTER_ITEM(SpeciesFilter, __VA_ARGS__ )

class SpeciesFilter: public Filter {
 public:
    FILTER_ITEM(asura, "asura")
    FILTER_ITEM(charr, "charr")
    FILTER_ITEM(human, "human")
    FILTER_ITEM(norn, "norn")
    FILTER_ITEM(sylvari, "sylvari")

    virtual string classname() { return "SpeciesFilter"; }
};


SpeciesFilter parse_SpeciesFilter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);

#undef FILTER_ITEM
