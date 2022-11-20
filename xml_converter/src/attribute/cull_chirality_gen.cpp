#include "cull_chirality_gen.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

CullChirality parse_cull_chirality(rapidxml::xml_attribute<>* input, vector<XMLError*>* errors) {
    CullChirality cull_chirality;
    string normalized_value = normalize(get_attribute_value(input));
    if (normalized_value == "none") {
        cull_chirality = CullChirality::none;
    }
    else if (normalized_value == "clockwise") {
        cull_chirality = CullChirality::clockwise;
    }
    else if (normalized_value == "counterclockwise") {
        cull_chirality = CullChirality::counter_clockwise;
    }
    else {
        errors->push_back(new XMLAttributeValueError("Found an invalid value that was not in the Enum CullChirality", input));
        cull_chirality = CullChirality::none;
    }
    return cull_chirality;
}

string stringify_cull_chirality(CullChirality attribute_value) {
    if (attribute_value == CullChirality::none) {
        return "none";
    }
    else if (attribute_value == CullChirality::clockwise) {
        return "clockwise";
    }
    else if (attribute_value == CullChirality::counter_clockwise) {
        return "counterclockwise";
    }
    else {
        return "CullChirality::none";
    }
}
