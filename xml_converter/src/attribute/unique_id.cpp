#include "unique_id.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

UniqueId parse_unique_id(rapidxml::xml_attribute<>* input, vector<XMLError*> *) {
    UniqueId unique_id;
    string base64;
    base64 = get_attribute_value(input);
    std::vector<uint8_t> guid = base64_decode(base64);
    unique_id.guid = guid;
    return unique_id;
}
