#include "trail_data_map_id.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

TrailDataMapId parse_trail_data_map_id(rapidxml::xml_attribute<>* input, vector<XMLError*> *) {
    TrailDataMapId trail_data_map_id;
    trail_data_map_id.trail_data_map_id = std::stof(get_attribute_value(input));
    return trail_data_map_id;
}

string stringify_trail_data_map_id(TrailDataMapId attribute_value) {
    return to_string(attribute_value.trail_data_map_id);
}
