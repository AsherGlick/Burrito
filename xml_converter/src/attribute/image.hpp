#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;
namespace waypoint {
class TexturePath;
}

class Image {
 public:
    std::string path;
};

void xml_attribute_to_image(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    Image* value,
    bool* is_set);

std::string image_to_xml_attribute(const std::string& attribute_name, const Image* value);

waypoint::TexturePath* to_proto_image(Image attribute_value);

Image from_proto_image(waypoint::TexturePath attribute_value);
