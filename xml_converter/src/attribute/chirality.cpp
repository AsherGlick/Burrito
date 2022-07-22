#include "chirality.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

Chirality parse_Chirality(rapidxml::xml_attribute<>* input, vector<XMLError*> *) {
    Chirality chirality;
    chirality.chirality = get_attribute_value(input);
    return chirality;
}
