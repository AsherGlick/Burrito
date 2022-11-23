#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

class XMLError;

class Image {
 public:
    std::string path;
};

Image parse_image(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_image(Image attribute_value);

waypoint::Texture* to_proto_image(Image attribute_value) ;
