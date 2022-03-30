#pragma once

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include <string>
#include <vector>
#include <map>

#include "filter.hpp"

using namespace std;


class FestivalFilter: public Filter {
public:
    FILTER_ITEM(dragonbash, "dragonbash")
    FILTER_ITEM(festival_of_the_four_winds, "festival_of_the_four_winds")
    FILTER_ITEM(haloween, "haloween")
    FILTER_ITEM(lunar_new_year, "lunar_new_year")
    FILTER_ITEM(super_adventure_festival, "super_adventure_festival")
    FILTER_ITEM(wintersday, "wintersday")
    FILTER_ITEM(none, "none")

    virtual string classname() { return "FestivalFilter"; }
};

FestivalFilter parse_FestivalFilter(rapidxml::xml_attribute<>* input, vector<string> *errors);
