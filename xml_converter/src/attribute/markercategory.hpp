#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

class MarkerCategory {

};

MarkerCategory parse_MarkerCategory(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
