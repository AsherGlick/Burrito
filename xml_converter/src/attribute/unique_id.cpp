#include "unique_id.hpp"

#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "waypoint.pb.h"

using namespace std;

UniqueId parse_unique_id(rapidxml::xml_attribute<>* input, vector<XMLError*>*) {
    UniqueId unique_id;
    string base64;
    base64 = get_attribute_value(input);
    std::vector<uint8_t> guid = base64_decode(base64);
    unique_id.guid = guid;
    return unique_id;
}

string stringify_unique_id(UniqueId attribute_value) {
    return base64_encode(&attribute_value.guid[0], attribute_value.guid.size());
}

string to_proto_unique_id(UniqueId attribute_value) {
    return std::string(attribute_value.guid.begin(), attribute_value.guid.end());
}

UniqueId from_proto_unique_id(string attribute_value) {
    UniqueId unique_id;
    std::vector<uint8_t> guid(attribute_value.begin(), attribute_value.end());
    unique_id.guid = guid;
    return unique_id;
}
