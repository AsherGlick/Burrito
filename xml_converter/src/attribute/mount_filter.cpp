#include "mount_filter.hpp"

using namespace std;

MountFilter parse_MountFilter(rapidxml::xml_attribute<>* input, vector<string> *errors) {
    MountFilter filter;
    filter.parse(input, errors);
    return filter;
}
