#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
class XMLError;
namespace waypoint {
class ProfessionFilter;
}

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
    }
};
void xml_attribute_to_profession_filter(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    ProfessionFilter* value,
    bool* is_set);
std::string profession_filter_to_xml_attribute(const std::string& attribute_name, const ProfessionFilter* value);

ProfessionFilter from_proto_profession_filter(waypoint::ProfessionFilter proto_profession_filter);

void profession_filter_to_proto(ProfessionFilter value, std::function<void(waypoint::ProfessionFilter*)> setter);
