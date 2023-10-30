#include "trail_data.hpp"

#include <stdint.h>

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "waypoint.pb.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// parse_trail_data
//
// Parses a TrailData from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
void xml_attribute_to_trail_data(
    rapidxml::xml_attribute<>* input,
    vector<XMLError*>* errors,
    string base_dir,
    TrailData* value,
    bool* is_set) {
    TrailData trail_data;
    string trail_data_relative_path = get_attribute_value(input);
    if (base_dir == "") {
        throw "Error: Marker pack base directory is an empty string";
    }
    if (trail_data_relative_path == "") {
        errors->push_back(new XMLAttributeValueError("Path to trail file is empty", input));
        return;
    }

    ifstream trail_data_file;
    string trail_path = base_dir + "/" + trail_data_relative_path;
    trail_data_file.open(trail_path, ios::in | ios::binary);
    if (!trail_data_file.good()) {
        errors->push_back(new XMLAttributeValueError("No trail file found at " + trail_path, input));
        return;
    }
    char version[4];
    trail_data_file.read(version, 4);
    // Validate the version number. Currently supports versions [0]
    if (!(*reinterpret_cast<uint32_t*>(version) == 0)) {
        errors->push_back(new XMLAttributeValueError("Unsupported version for trail data at " + trail_path, input));
        return;
    }

    char map_id_char[4];

    trail_data_file.read(map_id_char, 4);
    trail_data.side_effect_map_id = *reinterpret_cast<uint32_t*>(map_id_char);

    while (trail_data_file.tellg() > 0) {
        char point_x[4];
        trail_data_file.read(point_x, 4);
        trail_data.points_x.push_back(*reinterpret_cast<float*>(point_x));
        char point_y[4];
        trail_data_file.read(point_y, 4);
        trail_data.points_y.push_back(*reinterpret_cast<float*>(point_y));
        char point_z[4];
        trail_data_file.read(point_z, 4);
        trail_data.points_z.push_back(*reinterpret_cast<float*>(point_z));
    }

    if (trail_data.points_x.size() != trail_data.points_y.size() || trail_data.points_x.size() != trail_data.points_z.size()) {
        errors->push_back(new XMLAttributeValueError("Unexpected number of bits in trail file. Does not have equal number of X, Y, and Z coordinates." + trail_path, input));
    }

    trail_data_file.close();

    *value = trail_data;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// trail_data_to_xml_attribute
//
// Converts a traildata into a fully qualified xml attribute string.
// TODO: Write ".trl" files from data
// TOOD: Determine a better trail path name
////////////////////////////////////////////////////////////////////////////////
string trail_data_to_xml_attribute(const string& attribute_name, const TrailData* value) {
    return " " + attribute_name + "=\"" + "temp_name_of_trail.trl" + "\"";
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_trail_data
//
// Returns a waypoint::TrailData so that it can be saved to proto.
////////////////////////////////////////////////////////////////////////////////
waypoint::TrailData* to_proto_trail_data(TrailData attribute_value) {
    waypoint::TrailData* trail_data = new waypoint::TrailData();
    *trail_data->mutable_points_x() = {attribute_value.points_x.begin(), attribute_value.points_x.end()};
    *trail_data->mutable_points_y() = {attribute_value.points_y.begin(), attribute_value.points_y.end()};
    *trail_data->mutable_points_z() = {attribute_value.points_z.begin(), attribute_value.points_z.end()};
    return trail_data;
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_trail_data
//
// Returns a TrailData parsed from proto.
////////////////////////////////////////////////////////////////////////////////
TrailData from_proto_trail_data(waypoint::TrailData attribute_value) {
    TrailData trail_data;
    trail_data.points_x = {attribute_value.points_x().begin(), attribute_value.points_x().end()};
    trail_data.points_y = {attribute_value.points_y().begin(), attribute_value.points_y().end()};
    trail_data.points_z = {attribute_value.points_z().begin(), attribute_value.points_z().end()};
    return trail_data;
}
