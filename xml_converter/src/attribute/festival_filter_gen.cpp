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

string festival_filter_to_xml_attribute(const std::string& attribute_name, const FestivalFilter* value) {
    vector<string> flag_values;
    if (value->dragonbash == true) {
        flag_values.push_back("dragonbash");
    }
    if (value->festival_of_the_four_winds == true) {
        flag_values.push_back("festivalofthefourwinds");
    }
    if (value->halloween == true) {
        flag_values.push_back("halloween");
    }
    if (value->lunar_new_year == true) {
        flag_values.push_back("lunarnewyear");
    }
    if (value->super_adventure_festival == true) {
        flag_values.push_back("superadventurefestival");
    }
    if (value->wintersday == true) {
        flag_values.push_back("wintersday");
    }
    if (value->none == true) {
        flag_values.push_back("none");
    }
    string output = join(flag_values, ", ");
    return " " + attribute_name + "=\"" + output + "\"";
}

waypoint::FestivalFilter* to_proto_festival_filter(FestivalFilter attribute_value) {
    waypoint::FestivalFilter* proto_festival_filter = new waypoint::FestivalFilter();
    proto_festival_filter->set_dragonbash(attribute_value.dragonbash);
    proto_festival_filter->set_festival_of_the_four_winds(attribute_value.festival_of_the_four_winds);
    proto_festival_filter->set_halloween(attribute_value.halloween);
    proto_festival_filter->set_lunar_new_year(attribute_value.lunar_new_year);
    proto_festival_filter->set_super_adventure_festival(attribute_value.super_adventure_festival);
    proto_festival_filter->set_wintersday(attribute_value.wintersday);
    proto_festival_filter->set_none(attribute_value.none);
    return proto_festival_filter;
}

FestivalFilter from_proto_festival_filter(waypoint::FestivalFilter proto_festival_filter) {
    FestivalFilter festival_filter;
    festival_filter.dragonbash = proto_festival_filter.dragonbash();
    festival_filter.festival_of_the_four_winds = proto_festival_filter.festival_of_the_four_winds();
    festival_filter.halloween = proto_festival_filter.halloween();
    festival_filter.lunar_new_year = proto_festival_filter.lunar_new_year();
    festival_filter.super_adventure_festival = proto_festival_filter.super_adventure_festival();
    festival_filter.wintersday = proto_festival_filter.wintersday();
    festival_filter.none = proto_festival_filter.none();
    return festival_filter;
}
