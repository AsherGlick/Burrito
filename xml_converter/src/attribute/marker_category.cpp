#include "marker_category.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

MarkerCategory parse_marker_category(rapidxml::xml_attribute<>* input, vector<XMLError*> *) {
    MarkerCategory marker_category;
    marker_category.category = get_attribute_value(input);
    return marker_category;
}