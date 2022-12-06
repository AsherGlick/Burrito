#include "marker_category.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

////////////////////////////////////////////////////////////////////////////////
// parse_marker_category
//
// Parses a MarkerCategory from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
MarkerCategory parse_marker_category(rapidxml::xml_attribute<>* input, std::vector<XMLError*>*) {
    MarkerCategory marker_category;
    marker_category.category = get_attribute_value(input);
    return marker_category;
}

////////////////////////////////////////////////////////////////////////////////
// stringify_marker_category
//
// Converts a MarkerCategory into a stringy value so that it can be saved to
// xml. The stringy value is a full hierarchical path to the MarkerCategory.
////////////////////////////////////////////////////////////////////////////////
std::string stringify_marker_category(MarkerCategory attribute_value) {
    return attribute_value.category;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_marker_category
//
// Returns a waypoint::Category so that it can be saved to proto. 
////////////////////////////////////////////////////////////////////////////////
waypoint::Category* to_proto_marker_category(MarkerCategory attribute_value) {
    waypoint::Category* category = new waypoint::Category();
    category->set_type(attribute_value.category);
    return category;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_marker_category
//
// Parses a waypoint::Category and returns a MarkerCategory
///////////////////////////////////////////////////////////////////////////////
MarkerCategory from_proto_marker_category(waypoint::Category attribute_value) {
    MarkerCategory marker_category;
    marker_category.category = attribute_value.type();
    return marker_category;
}
