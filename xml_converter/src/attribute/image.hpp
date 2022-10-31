#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

class Image {
 public:
    string path;
};

Image parse_image(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);

string stringify_image (Image attribute_value);