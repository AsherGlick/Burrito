#include "marker_category.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "guildpoint.pb.h"

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Parses a MarkerCategory from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::MarkerCategory::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>*,
    XMLReaderState*,
    MarkerCategory* value,
    bool* is_set
) {
    value->category = get_attribute_value(input);
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Converts a marker category a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
void Attribute::MarkerCategory::to_xml_attribute(
    XMLWriterState*,
    const MarkerCategory* value,
    std::function<void(std::string)> setter
) {
    setter(value->category);
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Parses a marker category from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::MarkerCategory::from_proto_field(
    guildpoint::Category input,
    ProtoReaderState*,
    MarkerCategory* value,
    bool* is_set
) {
    MarkerCategory marker_category;
    marker_category.category = input.name();
    *value = marker_category;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Returns a guildpoint::Category so that it can be saved to proto.
////////////////////////////////////////////////////////////////////////////////
void Attribute::MarkerCategory::to_proto_field(
    MarkerCategory value,
    ProtoWriterState*,
    std::function<void(guildpoint::Category*)> setter
) {
    guildpoint::Category* category = new guildpoint::Category();
    category->set_name(value.category);
    setter(category);
}
