#include "profession_filter.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

ProfessionFilter parse_ProfessionFilter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors) {
    ProfessionFilter profession_filter;
    profession_filter.parse(input, errors);
    return profession_filter;
}
