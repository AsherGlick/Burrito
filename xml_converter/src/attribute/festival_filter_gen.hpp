#pragma once

#include <string>
#include <vector>
#include <functional>

#include "../rapidxml-1.13/rapidxml.hpp"
class XMLError;
namespace waypoint {
class FestivalFilter;
}

class FestivalFilter {
 public:
    bool dragonbash;
    bool festival_of_the_four_winds;
    bool halloween;
    bool lunar_new_year;
    bool none;
    bool super_adventure_festival;
    bool wintersday;

    virtual std::string classname() {
        return "FestivalFilter";
    }
};
void xml_attribute_to_festival_filter(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    FestivalFilter* value,
    bool* is_set);
std::string festival_filter_to_xml_attribute(const std::string& attribute_name, const FestivalFilter* value);

FestivalFilter from_proto_festival_filter(waypoint::FestivalFilter proto_festival_filter);

void festival_filter_to_proto(FestivalFilter value, std::function<void(waypoint::FestivalFilter*)> setter);
