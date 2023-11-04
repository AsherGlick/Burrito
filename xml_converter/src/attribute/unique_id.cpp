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

void xml_attribute_to_unique_id(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    UniqueId* value,
    bool* is_set) {
    string base64;
    base64 = get_attribute_value(input);
    std::vector<uint8_t> guid = base64_decode(base64);
    value->guid = guid;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// unique_id_to_xml_attribute
//
// Converts a unique id into a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
string unique_id_to_xml_attribute(const string& attribute_name, const UniqueId* value) {
    return " " + attribute_name + "=\"" + base64_encode(&(value->guid[0]), value->guid.size()) + "\"";
}


UniqueId from_proto_unique_id(string attribute_value) {
    UniqueId unique_id;
    std::vector<uint8_t> guid(attribute_value.begin(), attribute_value.end());
    unique_id.guid = guid;
    return unique_id;
}

////////////////////////////////////////////////////////////////////////////////
// unique_id_to_proto
//
// Writes a bool to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void unique_id_to_proto(UniqueId value, std::function<void(std::string)> setter) {
    setter(std::string(value.guid.begin(), value.guid.end()));
}
