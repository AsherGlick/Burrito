#include "unique_id.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

UniqueId parse_unique_id(rapidxml::xml_attribute<>* input, vector<XMLError*> *) {
    UniqueId unique_id;
    unique_id.guid = get_attribute_value(input);
    return unique_id;
}
