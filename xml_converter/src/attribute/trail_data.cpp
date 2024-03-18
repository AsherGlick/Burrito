#include "trail_data.hpp"

#include <stdint.h>

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "../packaging_xml.hpp"
#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
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
    XMLReaderState* state,
    TrailData* value,
    bool* is_set,
    int* map_id_value,
    bool* is_map_id_set) {
    TrailData trail_data;
    string trail_data_relative_path = get_attribute_value(input);
    if (state->marker_pack_root_directory == "") {
        throw "Error: Marker pack base directory is an empty string";
    }
    if (trail_data_relative_path == "") {
        errors->push_back(new XMLAttributeValueError("Path to trail file is empty", input));
        return;
    }

    ifstream trail_data_file;
    string trail_path = join_file_paths(state->marker_pack_root_directory, trail_data_relative_path);
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
    *map_id_value = *reinterpret_cast<uint32_t*>(map_id_char);
    *is_map_id_set = true;

    char points[12];
    while (trail_data_file.read(points, 12)) {
        trail_data.points_x.push_back(*reinterpret_cast<float*>(points));
        trail_data.points_y.push_back(*reinterpret_cast<float*>(points + 4));
        trail_data.points_z.push_back(*reinterpret_cast<float*>(points + 8));
    }

    if (trail_data_file.gcount() != 0) {
        errors->push_back(new XMLAttributeValueError("Unexpected number of bytes in trail file." + trail_path, input));
    }

    trail_data_file.close();

    *value = trail_data;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// djb2_hash
//
// A simple non cryptographic hash that we use to deterministically generate the
// filename of the trl files.
////////////////////////////////////////////////////////////////////////////////
uint64_t djb2_hash(const unsigned char* str, size_t length) {
    uint64_t hash = 5381;
    for (size_t i = 0; i < length; i++) {
        hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */
    }
    return hash;
}

////////////////////////////////////////////////////////////////////////////////
// trail_data_to_xml_attribute
//
// Converts a traildata into a fully qualified xml attribute string.
// TODO: Write ".trl" files from data
// TOOD: Determine a better trail path name
////////////////////////////////////////////////////////////////////////////////
uint32_t trail_version_number = 0;
string trail_data_to_xml_attribute(
    const string& attribute_name,
    XMLWriterState* state,
    const TrailData* value,
    const int* map_id_value,
    const bool* is_map_id_set) {
    size_t byte_array_size = sizeof(int) + sizeof(uint32_t) + value->points_x.size() * 3 * sizeof(float);
    unsigned char* byte_array = new unsigned char[byte_array_size];

    size_t offset = 0;
    std::memcpy(byte_array + offset, &trail_version_number, sizeof(trail_version_number));
    offset += sizeof(trail_version_number);

    std::memcpy(byte_array + offset, map_id_value, sizeof(*map_id_value));
    offset += sizeof(*map_id_value);

    for (size_t i = 0; i < value->points_x.size(); i++) {
        std::memcpy(byte_array + offset, &value->points_x[i], sizeof(float));
        offset += sizeof(float);
        std::memcpy(byte_array + offset, &value->points_y[i], sizeof(float));
        offset += sizeof(float);
        std::memcpy(byte_array + offset, &value->points_z[i], sizeof(float));
        offset += sizeof(float);
    }

    // Sanity check offset is where we think it should be.
    if (offset != byte_array_size) {
        cerr << "Found more data to write then we thought. This might mean there is a programming issue for serializing trl files." << endl;
        cerr << "Found " << offset << " instead of " << byte_array_size << endl;
    }

    string trail_file_name = long_to_hex_string(djb2_hash(byte_array, byte_array_size)) + ".trl";
    string trail_file_path = join_file_paths(state->marker_pack_root_directory, trail_file_name);

    ofstream trail_data_file(trail_file_path, ios::binary);

    if (!trail_data_file.good()) {
        cerr << "Error opening file. " << trail_file_path << endl;
    }
    trail_data_file.write(reinterpret_cast<const char*>(byte_array), byte_array_size);
    trail_data_file.close();

    delete[] byte_array;
    return " " + attribute_name + "=\"" + trail_file_name + "\"";
}

////////////////////////////////////////////////////////////////////////////////
// proto_to_trail_data
//
// Parses a TrailData from a proto field.
////////////////////////////////////////////////////////////////////////////////
void proto_to_trail_data(
    waypoint::TrailData input,
    ProtoReaderState*,
    TrailData* value,
    bool* is_set) {
    TrailData trail_data;
    trail_data.points_x = {input.points_x().begin(), input.points_x().end()};
    trail_data.points_y = {input.points_y().begin(), input.points_y().end()};
    trail_data.points_z = {input.points_z().begin(), input.points_z().end()};
    *value = trail_data;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// trail_data_to_proto
//
// Saves a TrailData object to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void trail_data_to_proto(
    TrailData value,
    ProtoWriterState*,
    std::function<void(waypoint::TrailData*)> setter) {
    waypoint::TrailData* trail_data = new waypoint::TrailData();
    *trail_data->mutable_points_x() = {value.points_x.begin(), value.points_x.end()};
    *trail_data->mutable_points_y() = {value.points_y.begin(), value.points_y.end()};
    *trail_data->mutable_points_z() = {value.points_z.begin(), value.points_z.end()};
    setter(trail_data);
}
