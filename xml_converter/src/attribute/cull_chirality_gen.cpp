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

CullChirality parse_cull_chirality(rapidxml::xml_attribute<>* input, vector<XMLError*>* errors) {
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
    return cull_chirality;
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

waypoint::CullChirality to_proto_cull_chirality(CullChirality attribute_value) {
    switch (attribute_value) {
        case CullChirality::none:
            return waypoint::CullChirality::none;
        case CullChirality::clockwise:
            return waypoint::CullChirality::clockwise;
        case CullChirality::counter_clockwise:
            return waypoint::CullChirality::counter_clockwise;
        default:
            return waypoint::CullChirality::none;
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
