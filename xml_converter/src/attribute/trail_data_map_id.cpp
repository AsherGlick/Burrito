#include "trail_data_map_id.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

// TODO: trail_data_map_id is currently unused in the convertor

////////////////////////////////////////////////////////////////////////////////
// parse_trail_data
//
// Parses a TrailDataMapID from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
TrailDataMapId parse_trail_data_map_id(rapidxml::xml_attribute<>* input, vector<XMLError*>*) {
    TrailDataMapId trail_data_map_id;
    trail_data_map_id.trail_data_map_id = std::stof(get_attribute_value(input));
    return trail_data_map_id;
}

////////////////////////////////////////////////////////////////////////////////
// stringify_trail_data
//
// Converts a TrailDataMapID into a stringy value so that it can be saved to xml
////////////////////////////////////////////////////////////////////////////////
string stringify_trail_data_map_id(TrailDataMapId attribute_value) {
    return to_string(attribute_value.trail_data_map_id);
}
