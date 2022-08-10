#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

class CullChirality {
 public:
    string cull_chirality;

    virtual string classname() { return "CullChirality"; };
};

CullChirality parse_CullChirality(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);