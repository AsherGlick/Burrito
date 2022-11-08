#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

class Color {
 public:
    std::string hex;
};

Color parse_color(rapidxml::xml_attribute<>* input, std::vector<XMLError*> *errors);

std::string stringify_color (Color attribute_value);