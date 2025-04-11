#include "image.hpp"

#include <filesystem>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

Attribute::Image::Image::Image()
    : filepath("", "") {
}

Attribute::Image::Image::Image(std::string base, std::string relative_filepath)
    : filepath(base, relative_filepath) {
}

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads an Image from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Image::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>*,
    XMLReaderState* state,
    Image* value,
    bool* is_set
) {
    Image image(state->marker_pack_root_directory, get_attribute_value(input));
    *value = image;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Writes an Image to an xml attribute using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Image::to_xml_attribute(
    XMLWriterState* state,
    const Image* value,
    std::function<void(std::string)> setter
) {
    MarkerPackFile output_path = MarkerPackFile(state->marker_pack_root_directory, value->filepath.relative_filepath);
    auto file_lookup = state->written_textures.find(output_path.tmp_get_path());
    if (file_lookup == state->written_textures.end()) {
        copy_file(value->filepath, output_path);
        state->written_textures.insert(output_path.tmp_get_path());
    }
    setter(value->filepath.relative_filepath);
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads an Image from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Image::from_proto_field(
    unsigned int input,
    ProtoReaderState* state,
    Image* value,
    bool* is_set
) {
    Image image(state->marker_pack_root_directory, state->textures[input].filepath());
    *value = image;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes an Image to a proto using the provided setter function. If the image
// has not been used before the image is added to the writer state before it is
// written to the proto setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Image::to_proto_field(
    const Image& value,
    ProtoWriterState* state,
    std::function<void(unsigned int)> setter
) {
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
