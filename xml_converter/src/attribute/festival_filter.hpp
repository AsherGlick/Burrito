#pragma once

#include <map>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "filter.hpp"

using namespace std;

#define FILTER_ITEM(...) CLASS_FILTER_ITEM(FestivalFilter, __VA_ARGS__ )

class FestivalFilter: public Filter {
 public:
    FILTER_ITEM(dragonbash, "dragonbash")
    FILTER_ITEM(festival_of_the_four_winds, "festivalofthefourwinds")
    FILTER_ITEM(halloween, "halloween")
    FILTER_ITEM(lunar_new_year, "lunarnewyear")
    FILTER_ITEM(super_adventure_festival, "superadventurefestival")
    FILTER_ITEM(wintersday, "wintersday")
    FILTER_ITEM(none, "none")

    virtual string classname() { return "FestivalFilter"; }
};

#undef FILTER_ITEM

FestivalFilter parse_FestivalFilter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
