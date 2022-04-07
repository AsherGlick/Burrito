#include "race_filter.hpp"

using namespace std;

RaceFilter parse_RaceFilter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors) {
    RaceFilter filter;
    filter.parse(input, errors);
    return filter;
}
