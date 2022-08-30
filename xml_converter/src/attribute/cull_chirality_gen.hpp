#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;
enum CullChirality {
    clockwise,
    counter_clockwise,
    none,
};
CullChirality parse_cull_chirality(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
string stringify_cull_chirality(CullChirality attribute_value);