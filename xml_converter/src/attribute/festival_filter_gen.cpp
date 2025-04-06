#include "festival_filter_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "guildpoint.pb.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads a FestivalFilter from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::FestivalFilter::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    FestivalFilter* value,
    bool* is_set
) {
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

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Writes a FestivalFilter to an xml attribute using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::FestivalFilter::to_xml_attribute(
    XMLWriterState*,
    const FestivalFilter* value,
    std::function<void(std::string)> setter
) {
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
    string output = join(flag_values, ",");
    setter(output);
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads a FestivalFilter from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::FestivalFilter::from_proto_field(
    guildpoint::FestivalFilter input,
    ProtoReaderState*,
    FestivalFilter* value,
    bool* is_set
) {
    FestivalFilter festival_filter;
    festival_filter.dragonbash = input.dragonbash();
    festival_filter.festival_of_the_four_winds = input.festival_of_the_four_winds();
    festival_filter.halloween = input.halloween();
    festival_filter.lunar_new_year = input.lunar_new_year();
    festival_filter.super_adventure_festival = input.super_adventure_festival();
    festival_filter.wintersday = input.wintersday();
    festival_filter.none = input.none();
    *value = festival_filter;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes a FestivalFilter to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::FestivalFilter::to_proto_field(
    FestivalFilter value,
    ProtoWriterState*,
    std::function<void(guildpoint::FestivalFilter*)> setter
) {
    guildpoint::FestivalFilter* proto_festival_filter = new guildpoint::FestivalFilter();
    bool should_write = false;
    proto_festival_filter->set_dragonbash(value.dragonbash);
    should_write |= value.dragonbash;
    proto_festival_filter->set_festival_of_the_four_winds(value.festival_of_the_four_winds);
    should_write |= value.festival_of_the_four_winds;
    proto_festival_filter->set_halloween(value.halloween);
    should_write |= value.halloween;
    proto_festival_filter->set_lunar_new_year(value.lunar_new_year);
    should_write |= value.lunar_new_year;
    proto_festival_filter->set_super_adventure_festival(value.super_adventure_festival);
    should_write |= value.super_adventure_festival;
    proto_festival_filter->set_wintersday(value.wintersday);
    should_write |= value.wintersday;
    proto_festival_filter->set_none(value.none);
    should_write |= value.none;
    if (should_write) {
        setter(proto_festival_filter);
    }
}
