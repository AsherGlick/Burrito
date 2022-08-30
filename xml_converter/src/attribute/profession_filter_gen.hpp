#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;
class ProfessionFilter {
 public:
    bool elementalist;
    bool engineer;
    bool guardian;
    bool mesmer;
    bool necromancer;
    bool ranger;
    bool revenant;
    bool thief;
    bool warrior;

    virtual string classname() { return "ProfessionFilter"; };
};
ProfessionFilter parse_profession_filter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
string stringify_profession_filter(ProfessionFilter attribute_value);