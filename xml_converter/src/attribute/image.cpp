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
Image parse_image(rapidxml::xml_attribute<>* input, vector<XMLError*>*) {
    Image image;
    image.path = get_attribute_value(input);
    return image;
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
// to_proto_image
//
// Converts an Image into a waypoint::Image pointer to save to proto.
////////////////////////////////////////////////////////////////////////////////
waypoint::TexturePath* to_proto_image(Image attribute_value) {
    waypoint::TexturePath* texture = new waypoint::TexturePath();
    texture->set_path(attribute_value.path);
    return texture;
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_image
//
// Parses a waypoint::Image from proto
////////////////////////////////////////////////////////////////////////////////
Image from_proto_image(waypoint::TexturePath attribute_value) {
    Image image;
    image.path = attribute_value.path();
    return image;
}
