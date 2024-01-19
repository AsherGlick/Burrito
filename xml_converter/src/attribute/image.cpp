#include "image.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// parse_image
//
// Parses the path to an image from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
void xml_attribute_to_image(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    Image* value,
    bool* is_set) {
    Image image;
    image.filename = get_attribute_value(input);
    image.original_filepath = join_file_paths(state->xml_filedir, image.filename);
    *value = image;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// image_to_xml_attribute
//
// Converts an image into a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
string image_to_xml_attribute(
    const string& attribute_name,
    XMLWriterState* state,
    const Image* value) {
    state->textures.push_back(value);
    return " " + attribute_name + "=\"" + value->filename + "\"";
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
    Image image;
    image.filename = state->textures[input].filepath();
    image.original_filepath = join_file_paths(state->proto_filedir, image.filename);
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
    auto file_map_lookup = state->texture_path_to_textures_index.find(value.original_filepath);
    if (file_map_lookup != state->texture_path_to_textures_index.end()) {
        texture_index = file_map_lookup->second;
    }
    else {
        texture_index = state->textures.size();
        state->texture_path_to_textures_index[value.original_filepath] = texture_index;
        state->textures.push_back(&value);
    }

    setter(texture_index);
}
