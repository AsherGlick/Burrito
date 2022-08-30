#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

class Color {
 public:
    string hex;
};

Color parse_color(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);

string stringify_color (Color attribute_value);