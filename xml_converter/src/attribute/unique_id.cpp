#include "unique_id.hpp"

#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "guildpoint.pb.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads a UniqueId from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::UniqueId::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>*,
    XMLReaderState*,
    UniqueId* value,
    bool* is_set
) {
    string base64;
    base64 = get_attribute_value(input);
    std::vector<uint8_t> guid = base64_decode(base64);
    value->guid = guid;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Writes a UniqueId to an xml attribute using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::UniqueId::to_xml_attribute(
    XMLWriterState*,
    const UniqueId* value,
    std::function<void(std::string)> setter
) {
    setter(base64_encode(&(value->guid[0]), value->guid.size()));
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads a UniqueId from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::UniqueId::from_proto_field(
    std::string input,
    ProtoReaderState*,
    UniqueId* value,
    bool* is_set
) {
    UniqueId unique_id;
    std::vector<uint8_t> guid(input.begin(), input.end());
    unique_id.guid = guid;
    *value = unique_id;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes a UniqueId to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::UniqueId::to_proto_field(
    UniqueId value,
    ProtoWriterState*,
    std::function<void(std::string)> setter
) {
    setter(std::string(value.guid.begin(), value.guid.end()));
}
