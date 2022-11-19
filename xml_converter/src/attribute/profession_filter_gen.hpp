#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

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

    virtual std::string classname() {
        return "ProfessionFilter";
    };
};
ProfessionFilter parse_profession_filter(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);
std::string stringify_profession_filter(ProfessionFilter attribute_value);
