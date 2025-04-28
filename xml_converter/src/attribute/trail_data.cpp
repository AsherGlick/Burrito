#include "trail_data.hpp"

#include <stdint.h>

#include <cstddef>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "../file_helper.hpp"
#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "guildpoint.pb.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// djb2_hash
//
// A simple non cryptographic hash that we use to deterministically generate the
// filename of the trl files.
// TODO: Remove this in favor of a better naming scheme or move this to
//       `hash_helpers.cpp` instead of here.
////////////////////////////////////////////////////////////////////////////////
static uint64_t djb2_hash(const unsigned char* str, size_t length) {
    uint64_t hash = 5381;
    for (size_t i = 0; i < length; i++) {
        hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */
    }
    return hash;
}

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads a TrailData from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::TrailData::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    vector<XMLError*>* errors,
    XMLReaderState* state,
    TrailData* value,
    bool* is_set,
    int* map_id_value,
    bool* is_map_id_set
) {
    TrailData trail_data;
    string trail_data_relative_path = get_attribute_value(input);
    if (state->marker_pack_root_directory == "") {
        throw "Error: Marker pack base directory is an empty string";
    }
    if (trail_data_relative_path == "") {
        errors->push_back(new XMLAttributeValueError("Path to trail file is empty", input));
        return;
    }

    MarkerPackFile trail_file(state->marker_pack_root_directory, trail_data_relative_path);
    auto trail_data_file = read_file(trail_file);

    if (trail_data_file == nullptr) {
        errors->push_back(new XMLAttributeValueError("No trail file found at " + trail_file.tmp_get_path(), input));
        return;
    }

    char version_buffer[4];
    trail_data_file->read(version_buffer, 4);

    // Validate the version number. Currently supports versions [0]
    uint32_t version = *reinterpret_cast<uint32_t*>(version_buffer);
    if (!(version == 0)) {
        errors->push_back(new XMLAttributeValueError("Unsupported version (" + to_string(version) + ") for trail data at " + trail_file.tmp_get_path(), input));
        return;
    }

    char map_id_char[4];
    trail_data_file->read(map_id_char, 4);
    *map_id_value = *reinterpret_cast<uint32_t*>(map_id_char);
    *is_map_id_set = true;

    char points[12];
    while (trail_data_file->read(points, 12)) {
        float x = *reinterpret_cast<float*>(points);
        float y = *reinterpret_cast<float*>(points + 4);
        float z = *reinterpret_cast<float*>(points + 8);
        trail_data.points.push_back({x, y, z});
    }

    if (trail_data_file->gcount() != 0) {
        errors->push_back(new XMLAttributeValueError("Unexpected number of bytes in trail file." + trail_file.tmp_get_path(), input));
    }

    *value = trail_data;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Writes a TrailData to an xml attribute using the provided setter function.
// TOOD: Determine a better trail path name
////////////////////////////////////////////////////////////////////////////////
uint32_t trail_version_number = 0;
void Attribute::TrailData::to_xml_attribute(
    XMLWriterState* state,
    const TrailData* value,
    const int* map_id_value,
    const bool*,  // is_map_id_set
    std::function<void(std::string)> setter
) {
    size_t byte_array_size = sizeof(int) + sizeof(uint32_t) + value->points.size() * 3 * sizeof(float);
    unsigned char* byte_array = new unsigned char[byte_array_size];

    size_t offset = 0;
    std::memcpy(byte_array + offset, &trail_version_number, sizeof(trail_version_number));
    offset += sizeof(trail_version_number);

    std::memcpy(byte_array + offset, map_id_value, sizeof(*map_id_value));
    offset += sizeof(*map_id_value);

    for (size_t i = 0; i < value->points.size(); i++) {
        std::memcpy(byte_array + offset, &value->points[i].x, sizeof(float));
        offset += sizeof(float);
        std::memcpy(byte_array + offset, &value->points[i].y, sizeof(float));
        offset += sizeof(float);
        std::memcpy(byte_array + offset, &value->points[i].z, sizeof(float));
        offset += sizeof(float);
    }

    // Sanity check offset is where we think it should be.
    if (offset != byte_array_size) {
        cerr << "Found more data to write then we thought. This might mean there is a programming issue for serializing trl files." << endl;
        cerr << "Found " << offset << " instead of " << byte_array_size << endl;
    }

    string trail_file_name = long_to_hex_string(djb2_hash(byte_array, byte_array_size)) + ".trl";
    stringstream data;
    data.write(reinterpret_cast<const char*>(byte_array), byte_array_size);
    write_file({state->marker_pack_root_directory, trail_file_name}, data);

    delete[] byte_array;
    setter(trail_file_name);
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads a TrailData from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::TrailData::from_proto_field(
    guildpoint::TrailData input,
    ProtoReaderState*,
    TrailData* value,
    bool* is_set
) {
    TrailData trail_data;

    // TODO: Validate the size
    size_t size = input.points_x().size();
    trail_data.points.resize(size);

    for (size_t i = 0; i < size; i++) {
        trail_data.points[i] = {
            input.points_x()[i],
            input.points_y()[i],
            input.points_z()[i],
        };
    }

    *value = trail_data;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes a TrailData to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::TrailData::to_proto_field(
    TrailData value,
    ProtoWriterState*,
    std::function<void(guildpoint::TrailData*)> setter
) {
    guildpoint::TrailData* trail_data = new guildpoint::TrailData();

    size_t size = value.points.size();

    trail_data->mutable_points_x()->Reserve(size);
    trail_data->mutable_points_y()->Reserve(size);
    trail_data->mutable_points_z()->Reserve(size);

    for (size_t i = 0; i < size; i++) {
        trail_data->mutable_points_x()->Add(value.points[i].x);
        trail_data->mutable_points_y()->Add(value.points[i].y);
        trail_data->mutable_points_z()->Add(value.points[i].z);
    }

    setter(trail_data);
}
