#pragma once

#include <map>
#include <string>
#include <vector>

struct ProtoReaderState {
    // A map from the index within "textures" to the texture path.
    std::map<uint32_t, std::string> textures_index_to_texture_path;
    std::string proto_filedir;
    std::vector<std::string> all_output_dirs;
};
