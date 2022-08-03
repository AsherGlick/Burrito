#include "species_filter.hpp"

using namespace std;

SpeciesFilter parse_RaceFilter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors) {
    SpeciesFilter filter;
    filter.parse(input, errors);
    return filter;
}
