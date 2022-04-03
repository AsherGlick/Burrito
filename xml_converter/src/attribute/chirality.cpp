#include "chirality.hpp"

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

Chirality parse_Chirality(rapidxml::xml_attribute<>* input, vector<string> *) {
    Chirality chirality;
    chirality.chirality = string(input->value());
    return chirality;
}
