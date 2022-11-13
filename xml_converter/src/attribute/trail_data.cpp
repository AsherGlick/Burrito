#include "trail_data.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

TrailData parse_trail_data(rapidxml::xml_attribute<>* input, vector<XMLError*> *) {
    TrailData trail_data;
    trail_data.trail_data = get_attribute_value(input);
    return trail_data;
}

string stringify_trail_data(TrailData attribute_value) {
    return attribute_value.trail_data;
}
