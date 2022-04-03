#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

class Color {
 public:
    string hex;
};

Color parse_Color(rapidxml::xml_attribute<>* input, vector<string> *errors);
