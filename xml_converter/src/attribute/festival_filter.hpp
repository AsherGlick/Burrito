#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

class FestivalFilter {
 public:
    bool dragonbash;
    bool festival_of_the_four_winds;
    bool halloween;
    bool lunar_new_year;
    bool none;
    bool super_adventure_festival;
    bool wintersday;

    virtual string classname() { return "FestivalFilter"; };
};

FestivalFilter parse_FestivalFilter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);