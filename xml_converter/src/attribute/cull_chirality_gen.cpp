#include "cull_chirality_gen.hpp"

#include <algorithm>
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
// Reads a CullChirality from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::CullChirality::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    CullChirality* value,
    bool* is_set
) {
    CullChirality cull_chirality;
    string normalized_value = normalize(get_attribute_value(input));
    if (normalized_value == "none") {
        cull_chirality = CullChirality::none;
    }
    else if (normalized_value == "clockwise") {
        cull_chirality = CullChirality::clockwise;
    }
    else if (normalized_value == "counterclockwise") {
        cull_chirality = CullChirality::counter_clockwise;
    }
    else {
        errors->push_back(new XMLAttributeValueError("Found an invalid value that was not in the Enum CullChirality", input));
        cull_chirality = CullChirality::none;
    }
    *value = cull_chirality;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Writes a CullChirality to an xml attribute using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::CullChirality::to_xml_attribute(
    XMLWriterState*,
    const CullChirality* value,
    std::function<void(std::string)> setter
) {
    if (*value == CullChirality::none) {
        setter("none");
        return;
    }
    else if (*value == CullChirality::clockwise) {
        setter("clockwise");
        return;
    }
    else if (*value == CullChirality::counter_clockwise) {
        setter("counterclockwise");
        return;
    }
    else {
        setter("CullChirality::none");
    }
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads a CullChirality from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::CullChirality::from_proto_field(
    guildpoint::CullChirality input,
    ProtoReaderState*,
    CullChirality* value,
    bool* is_set
) {
    switch (input) {
        case guildpoint::CullChirality::none:
            *value = CullChirality::none;
            *is_set = true;
            break;
        case guildpoint::CullChirality::clockwise:
            *value = CullChirality::clockwise;
            *is_set = true;
            break;
        case guildpoint::CullChirality::counter_clockwise:
            *value = CullChirality::counter_clockwise;
            *is_set = true;
            break;
        default:
            *value = CullChirality::none;
            *is_set = true;
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes a CullChirality to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::CullChirality::to_proto_field(
    CullChirality value,
    ProtoWriterState*,
    std::function<void(guildpoint::CullChirality)> setter
) {
    switch (value) {
        case CullChirality::none:
            setter(guildpoint::CullChirality::none);
            break;
        case CullChirality::clockwise:
            setter(guildpoint::CullChirality::clockwise);
            break;
        case CullChirality::counter_clockwise:
            setter(guildpoint::CullChirality::counter_clockwise);
            break;
        default:
            setter(guildpoint::CullChirality::none);
            break;
    }
}
