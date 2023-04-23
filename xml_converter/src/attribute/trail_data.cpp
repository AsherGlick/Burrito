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
TrailData parse_trail_data(rapidxml::xml_attribute<>* input, vector<XMLError*>* errors, string base_dir) {
    TrailData trail_data;
    trail_data.trail_data_relative_path = get_attribute_value(input);
    if (base_dir == "" || trail_data.trail_data_relative_path == "") {
        errors->push_back(new XMLAttributeValueError("", input));
    }
    ifstream trail_data_file;
    string trail_path = base_dir + "/" + trail_data.trail_data_relative_path;
    trail_data_file.open(trail_path, ios::in | ios::binary);
    if (trail_data_file.good()) {
        char version[4];
        trail_data_file.read(version, 4);

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
    }
    else
        errors->push_back(new XMLAttributeValueError("No trail file found at " + trail_path, input));
    return trail_data;
}

////////////////////////////////////////////////////////////////////////////////
// stringify_trail_data
//
// Returns the relative path of the trail_data to the xml files
// TODO: Write ".trl" files from data
////////////////////////////////////////////////////////////////////////////////
string stringify_trail_data(TrailData attribute_value) {
    return attribute_value.trail_data_relative_path;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_trail_data
//
// Returns a waypoint::TrailData so that it can be saved to proto.
////////////////////////////////////////////////////////////////////////////////
waypoint::TrailData* to_proto_trail_data(TrailData attribute_value) {
    waypoint::TrailData* trail_data = new waypoint::TrailData();
    trail_data->set_trail_data_relative_path(attribute_value.trail_data_relative_path);
    trail_data->mutable_points_x()->Add(attribute_value.points_x.begin(), attribute_value.points_x.end());
    trail_data->mutable_points_y()->Add(attribute_value.points_y.begin(), attribute_value.points_y.end());
    trail_data->mutable_points_z()->Add(attribute_value.points_z.begin(), attribute_value.points_z.end());
    return trail_data;
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_trail_data
//
// Returns a TrailData parsed from proto.
////////////////////////////////////////////////////////////////////////////////
TrailData from_proto_trail_data(waypoint::TrailData attribute_value) {
    TrailData trail_data;
    trail_data.trail_data_relative_path = attribute_value.trail_data_relative_path();
    trail_data.points_x = {attribute_value.points_x().begin(), attribute_value.points_x().end()};
    trail_data.points_y = {attribute_value.points_y().begin(), attribute_value.points_y().end()};
    trail_data.points_z = {attribute_value.points_z().begin(), attribute_value.points_z().end()};
    return trail_data;
}
