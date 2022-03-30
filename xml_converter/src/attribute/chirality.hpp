#pragma once

#include "../rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>

using namespace std;


class Chirality {
public:
	string chirality;
};

Chirality parse_Chirality(rapidxml::xml_attribute<>* input, vector<string> *errors);
