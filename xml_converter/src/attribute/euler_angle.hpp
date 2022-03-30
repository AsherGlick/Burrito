#pragma once

#include "../rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>

using namespace std;


class EulerAngle {
public:
	float x;
	float y;
	float z;
};

EulerAngle parse_EulerAngle(rapidxml::xml_attribute<>* input, vector<string> *errors);
