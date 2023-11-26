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
    XMLReaderState* state,
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
string unique_id_to_xml_attribute(
    const string& attribute_name,
    XMLWriterState* state,
    const UniqueId* value) {
    return " " + attribute_name + "=\"" + base64_encode(&(value->guid[0]), value->guid.size()) + "\"";
}

////////////////////////////////////////////////////////////////////////////////
// proto_to_unique_id
//
// Parses a UniqueId from a proto field.
////////////////////////////////////////////////////////////////////////////////
void proto_to_unique_id(
    std::string input,
    ProtoReaderState* state,
    UniqueId* value,
    bool* is_set) {
    UniqueId unique_id;
    std::vector<uint8_t> guid(input.begin(), input.end());
    unique_id.guid = guid;
    *value = unique_id;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// unique_id_to_proto
//
// Writes a bool to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void unique_id_to_proto(
    UniqueId value,
    ProtoWriterState* state,
    std::function<void(std::string)> setter) {
    setter(std::string(value.guid.begin(), value.guid.end()));
}
