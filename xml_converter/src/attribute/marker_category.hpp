#pragma once

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

MarkerCategory parse_marker_category(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_marker_category(MarkerCategory attribute_value);

waypoint::Category* to_proto_marker_category(MarkerCategory attribute_value);

MarkerCategory from_proto_marker_category(waypoint::Category attribute_value);
