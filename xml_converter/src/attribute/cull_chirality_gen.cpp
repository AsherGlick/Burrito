#include "cull_chirality_gen.hpp"

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

string stringify_cull_chirality(CullChirality attribute_value) {
    if (attribute_value == CullChirality::none) {
        return "none";
    }
    else if (attribute_value == CullChirality::clockwise) {
        return "clockwise";
    }
    else if (attribute_value == CullChirality::counter_clockwise) {
        return "counterclockwise";
    }
    else {
        return "CullChirality::none";
    }
}

waypoint::CullChirality to_proto_cull_chirality(CullChirality attribute_value) {
    waypoint::CullChirality proto_cull_chirality;
    if (attribute_value == CullChirality::none) {
        proto_cull_chirality = waypoint::CullChirality::none;
    }
    else if (attribute_value == CullChirality::clockwise) {
        proto_cull_chirality = waypoint::CullChirality::clockwise;
    }
    else if (attribute_value == CullChirality::counter_clockwise) {
        proto_cull_chirality = waypoint::CullChirality::counter_clockwise;
    }
    else {
        proto_cull_chirality = waypoint::CullChirality::none;
    }
    return proto_cull_chirality;
}

CullChirality from_proto_cull_chirality(waypoint::CullChirality proto_cull_chirality) {
    CullChirality cull_chirality;
    if (proto_cull_chirality == waypoint::CullChirality::none) {
        cull_chirality = CullChirality::none;
    }
    else if (proto_cull_chirality == waypoint::CullChirality::clockwise) {
        cull_chirality = CullChirality::clockwise;
    }
    else if (proto_cull_chirality == waypoint::CullChirality::counter_clockwise) {
        cull_chirality = CullChirality::counter_clockwise;
    }
    else {
        cull_chirality = CullChirality::none;
    }
    return cull_chirality;
}