#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Attribute::Image {
class Image;
}

class ProtoWriterState {
 public:
    std::string marker_pack_root_directory;
    // A map from texture path to the index within "textures" that the path is saved in.
    std::map<std::string, uint32_t> texture_path_to_textures_index;

    // A list of all of the textures with their paths.
    std::vector<const Attribute::Image::Image*> textures;

    ProtoWriterState();
};
