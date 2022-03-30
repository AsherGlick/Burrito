#pragma once

#include "../rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>

using namespace std;

class Color {
public:
	string hex;
};

Color parse_Color(rapidxml::xml_attribute<>* input, vector<string> *errors);
