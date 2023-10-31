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

void xml_attribute_to_festival_filter(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    FestivalFilter* value,
    bool* is_set) {
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
    *value = festival_filter;
    *is_set = true;
}

string festival_filter_to_xml_attribute(const std::string& attribute_name, const FestivalFilter* value) {
    string output = "";
    if (value->dragonbash == true) {
        output = output + "dragonbash";
    }
    if (value->festival_of_the_four_winds == true) {
        output = output + "festivalofthefourwinds";
    }
    if (value->halloween == true) {
        output = output + "halloween";
    }
    if (value->lunar_new_year == true) {
        output = output + "lunarnewyear";
    }
    if (value->super_adventure_festival == true) {
        output = output + "superadventurefestival";
    }
    if (value->wintersday == true) {
        output = output + "wintersday";
    }
    if (value->none == true) {
        output = output + "none";
    }
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
