#include "specialization_filter.hpp"

SpecializationFilter parse_SpecializationFilter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors) {
    SpecializationFilter specialization_filter;
    specialization_filter.parse(input, errors);
    return specialization_filter;
}
