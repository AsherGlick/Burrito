#pragma once

#include "../rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>

using namespace std;


class Position {
public:
	float x;
	float y;
	float z;
};

Position parse_Position(rapidxml::xml_attribute<>* input, vector<string> *errors);
