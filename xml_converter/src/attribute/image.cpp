#include "image.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// parse_image
//
// Parses the path to an image from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
void xml_attribute_to_image(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    Image* value,
    bool* is_set) {
    value->path = get_attribute_value(input);
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// image_to_xml_attribute
//
// Converts an image into a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
string image_to_xml_attribute(const string& attribute_name, const Image* value) {
    return " " + attribute_name + "=\"" + value->path + "\"";
}

////////////////////////////////////////////////////////////////////////////////
// proto_to_image
//
// Parses an Image from proto
////////////////////////////////////////////////////////////////////////////////
void proto_to_image(waypoint::TexturePath input, Image* value, bool* is_set) {
    Image image;
    image.path = input.path();
    *value = image;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// image_to_proto
//
// Writes a string filepath to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void image_to_proto(Image value, std::function<void(waypoint::TexturePath*)> setter) {
    waypoint::TexturePath* texture = new waypoint::TexturePath();
    texture->set_path(value.path);
    setter(texture);
}
