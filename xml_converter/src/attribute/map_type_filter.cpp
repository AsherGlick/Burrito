#include "map_type_filter.hpp"

using namespace std;

MapTypeFilter parse_MapTypeFilter(rapidxml::xml_attribute<>* input, vector<string> *errors) {
    MapTypeFilter filter;
    filter.parse(input, errors);
    return filter;
}
