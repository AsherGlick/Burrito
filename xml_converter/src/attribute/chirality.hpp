#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;


class Chirality {
 public:
    string chirality;
};

Chirality parse_Chirality(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
