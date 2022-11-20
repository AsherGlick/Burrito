#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

class TrailData {
 public:
    std::string trail_data;
};

TrailData parse_trail_data(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_trail_data(TrailData attribute_value);
