#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;
namespace waypoint {
class Category;
}

class MarkerCategory {
 public:
    std::string category;
};

void xml_attribute_to_marker_category(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    MarkerCategory* value,
    bool* is_set);

std::string marker_category_to_xml_attribute(const std::string& attribute_name, const MarkerCategory* value);

void proto_to_marker_category(waypoint::Category input, MarkerCategory* value, bool* is_set);

void marker_category_to_proto(MarkerCategory value, std::function<void(waypoint::Category*)> setter);
