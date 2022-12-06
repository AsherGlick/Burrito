#include "festival_filter_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "waypoint.pb.h"

using namespace std;

FestivalFilter parse_festival_filter(rapidxml::xml_attribute<>* input, vector<XMLError*>* errors) {
    FestivalFilter festival_filter;
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");
    festival_filter.dragonbash = false;
    festival_filter.festival_of_the_four_winds = false;
    festival_filter.halloween = false;
    festival_filter.lunar_new_year = false;
    festival_filter.super_adventure_festival = false;
    festival_filter.wintersday = false;
    festival_filter.none = false;

    for (string flag_value : flag_values) {
        string normalized_flag_value = normalize(flag_value);
        if (normalized_flag_value == "dragonbash") {
            festival_filter.dragonbash = true;
        }
        else if (normalized_flag_value == "festivalofthefourwinds") {
            festival_filter.festival_of_the_four_winds = true;
        }
        else if (normalized_flag_value == "halloween") {
            festival_filter.halloween = true;
        }
        else if (normalized_flag_value == "lunarnewyear") {
            festival_filter.lunar_new_year = true;
        }
        else if (normalized_flag_value == "superadventurefestival") {
            festival_filter.super_adventure_festival = true;
        }
        else if (normalized_flag_value == "superadventurebox") {
            festival_filter.super_adventure_festival = true;
        }
        else if (normalized_flag_value == "wintersday") {
            festival_filter.wintersday = true;
        }
        else if (normalized_flag_value == "none") {
            festival_filter.none = true;
        }
        else {
            errors->push_back(new XMLAttributeValueError("Invalid Filter for FestivalFilter. Found " + flag_value, input));
            continue;
        }
    }
    return festival_filter;
}

string stringify_festival_filter(FestivalFilter attribute_value) {
    string output = "";
    if (attribute_value.dragonbash == true) {
        output = output + "dragonbash";
    }
    if (attribute_value.festival_of_the_four_winds == true) {
        output = output + "festivalofthefourwinds";
    }
    if (attribute_value.halloween == true) {
        output = output + "halloween";
    }
    if (attribute_value.lunar_new_year == true) {
        output = output + "lunarnewyear";
    }
    if (attribute_value.super_adventure_festival == true) {
        output = output + "superadventurefestival";
    }
    if (attribute_value.wintersday == true) {
        output = output + "wintersday";
    }
    if (attribute_value.none == true) {
        output = output + "none";
    }
    return output;
}

waypoint::FestivalFilter* to_proto_festival_filter(FestivalFilter attribute_value) {
    waypoint::FestivalFilter* proto_festival_filter = new waypoint::FestivalFilter();
    if (attribute_value.dragonbash == true) {
        proto_festival_filter->set_dragonbash(true);
    }
    if (attribute_value.festival_of_the_four_winds == true) {
        proto_festival_filter->set_festival_of_the_four_winds(true);
    }
    if (attribute_value.halloween == true) {
        proto_festival_filter->set_halloween(true);
    }
    if (attribute_value.lunar_new_year == true) {
        proto_festival_filter->set_lunar_new_year(true);
    }
    if (attribute_value.super_adventure_festival == true) {
        proto_festival_filter->set_super_adventure_festival(true);
    }
    if (attribute_value.wintersday == true) {
        proto_festival_filter->set_wintersday(true);
    }
    if (attribute_value.none == true) {
        proto_festival_filter->set_none(true);
    }
    return proto_festival_filter;
}

FestivalFilter from_proto_festival_filter(waypoint::FestivalFilter proto_festival_filter) {
    FestivalFilter festival_filter;
    if (proto_festival_filter.dragonbash() == true) {
        festival_filter.dragonbash = true;
    }
    if (proto_festival_filter.festival_of_the_four_winds() == true) {
        festival_filter.festival_of_the_four_winds = true;
    }
    if (proto_festival_filter.halloween() == true) {
        festival_filter.halloween = true;
    }
    if (proto_festival_filter.lunar_new_year() == true) {
        festival_filter.lunar_new_year = true;
    }
    if (proto_festival_filter.super_adventure_festival() == true) {
        festival_filter.super_adventure_festival = true;
    }
    if (proto_festival_filter.wintersday() == true) {
        festival_filter.wintersday = true;
    }
    if (proto_festival_filter.none() == true) {
        festival_filter.none = true;
    }
    return festival_filter;
}
