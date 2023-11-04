#pragma once

#include <string>
#include <vector>
#include <functional>

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

Image from_proto_image(waypoint::TexturePath attribute_value);

void image_to_proto(Image value, std::function<void(waypoint::TexturePath*)> setter);
