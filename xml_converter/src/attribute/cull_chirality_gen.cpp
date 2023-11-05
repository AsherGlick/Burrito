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

string cull_chirality_to_xml_attribute(const std::string& attribute_name, const CullChirality* value) {
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

CullChirality from_proto_cull_chirality(waypoint::CullChirality proto_cull_chirality) {
    switch (proto_cull_chirality) {
        case waypoint::CullChirality::none:
            return CullChirality::none;
        case waypoint::CullChirality::clockwise:
            return CullChirality::clockwise;
        case waypoint::CullChirality::counter_clockwise:
            return CullChirality::counter_clockwise;
        default:
            return CullChirality::none;
    }
}

void cull_chirality_to_proto(CullChirality value, std::function<void(waypoint::CullChirality)> setter) {
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
