#include "marker_category.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "guildpoint.pb.h"

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads a MarkerCategory from an xml attribute.
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
// Writes a MarkerCategory to an xml attribute using the provided setter function.
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
// Reads a MarkerCategory from a proto field.
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
// Writes a MarkerCategory to a proto using the provided setter function.
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
