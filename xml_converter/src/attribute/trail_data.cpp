#include "trail_data.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// parse_trail_data
//
// Parses a TrailData from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
TrailData parse_trail_data(rapidxml::xml_attribute<>* input, vector<XMLError*>*) {
    TrailData trail_data;
    trail_data.trail_data = get_attribute_value(input);
    return trail_data;
}

////////////////////////////////////////////////////////////////////////////////
// stringify_trail_data
//
// Converts a TrailData into a stringy value so that it can be saved to xml
////////////////////////////////////////////////////////////////////////////////
string stringify_trail_data(TrailData attribute_value) {
    return attribute_value.trail_data;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_trail_data
//
// Returns a waypoint::TrailData so that it can be saved to proto. 
////////////////////////////////////////////////////////////////////////////////
waypoint::TrailData* to_proto_trail_data(TrailData attribute_value) {
    waypoint::TrailData* trail_data = new waypoint::TrailData();
    trail_data->set_trail_data(attribute_value.trail_data);
    return trail_data;
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_trail_data
//
// Returns a TrailData parsed from proto. 
////////////////////////////////////////////////////////////////////////////////
TrailData from_proto_trail_data(waypoint::TrailData attribute_value) {
    TrailData trail_data;
    trail_data.trail_data = attribute_value.trail_data();
    return trail_data;
}