#include "image.hpp"

#include <filesystem>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

Image::Image()
    : filepath("", "") {
}

Image::Image(std::string base, std::string relative_filepath)
    : filepath(base, relative_filepath) {
}

////////////////////////////////////////////////////////////////////////////////
// parse_image
//
// Parses the path to an image from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
void xml_attribute_to_image(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>*,
    XMLReaderState* state,
    Image* value,
    bool* is_set) {
    Image image(state->marker_pack_root_directory, get_attribute_value(input));
    *value = image;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// image_to_xml_attribute
//
// Converts an image into a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
void image_to_xml_attribute(
    XMLWriterState* state,
    const Image* value,
    std::function<void(std::string)> setter) {
    MarkerPackFile output_path = MarkerPackFile(state->marker_pack_root_directory, value->filepath.relative_filepath);
    copy_file(value->filepath, output_path);
    setter(value->filepath.relative_filepath);
}

////////////////////////////////////////////////////////////////////////////////
// proto_to_image
//
// Parses an Image from proto
////////////////////////////////////////////////////////////////////////////////
void proto_to_image(
    unsigned int input,
    ProtoReaderState* state,
    Image* value,
    bool* is_set) {
    Image image(state->marker_pack_root_directory, state->textures[input].filepath());
    *value = image;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// image_to_proto
//
// Creates a new element of the proto writer state if the image has not been
// used before. Then writes the new or existing index of the image to the proto.
////////////////////////////////////////////////////////////////////////////////
void image_to_proto(
    const Image& value,
    ProtoWriterState* state,
    std::function<void(unsigned int)> setter) {
    // Get the texture index or create a new one
    uint32_t texture_index = 0;
    auto file_map_lookup = state->texture_path_to_textures_index.find(value.filepath.tmp_get_path());
    if (file_map_lookup != state->texture_path_to_textures_index.end()) {
        texture_index = file_map_lookup->second;
    }
    else {
        texture_index = state->textures.size();
        state->texture_path_to_textures_index[value.filepath.tmp_get_path()] = texture_index;
        state->textures.push_back(&value);

        MarkerPackFile output_path = MarkerPackFile(state->marker_pack_root_directory, value.filepath.relative_filepath);
        copy_file(value.filepath, output_path);
    }
    setter(texture_index);
}
