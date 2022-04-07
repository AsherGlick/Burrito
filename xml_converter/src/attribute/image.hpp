#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

class Image {
 public:
    string path;
    rapidxml::xml_attribute<>* original_token;
};

Image parse_Image(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);

