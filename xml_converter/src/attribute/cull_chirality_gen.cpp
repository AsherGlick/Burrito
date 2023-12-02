#include "cull_chirality_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "waypoint.pb.h"

using namespace std;

void xml_attribute_to_cull_chirality(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    CullChirality* value,
    bool* is_set) {
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

string cull_chirality_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const CullChirality* value) {
    if (*value == CullChirality::none) {
        return " " + attribute_name + "=\"" + "none" + "\"";
    }
    else if (*value == CullChirality::clockwise) {
        return " " + attribute_name + "=\"" + "clockwise" + "\"";
    }
    else if (*value == CullChirality::counter_clockwise) {
        return " " + attribute_name + "=\"" + "counterclockwise" + "\"";
    }
    else {
        return " " + attribute_name + "=\"" + "CullChirality::none" + "\"";
    }
}

void proto_to_cull_chirality(
    waypoint::CullChirality input,
    ProtoReaderState* state,
    CullChirality* value,
    bool* is_set) {
    switch (input) {
        case waypoint::CullChirality::none:
            *value = CullChirality::none;
            *is_set = true;
            break;
        case waypoint::CullChirality::clockwise:
            *value = CullChirality::clockwise;
            *is_set = true;
            break;
        case waypoint::CullChirality::counter_clockwise:
            *value = CullChirality::counter_clockwise;
            *is_set = true;
            break;
        default:
            *value = CullChirality::none;
            *is_set = true;
            break;
    }
}

void cull_chirality_to_proto(
    CullChirality value,
    ProtoWriterState* state,
    std::function<void(waypoint::CullChirality)> setter) {
    switch (value) {
        case CullChirality::none:
            setter(waypoint::CullChirality::none);
            break;
        case CullChirality::clockwise:
            setter(waypoint::CullChirality::clockwise);
            break;
        case CullChirality::counter_clockwise:
            setter(waypoint::CullChirality::counter_clockwise);
            break;
        default:
            setter(waypoint::CullChirality::none);
            break;
    }
}
