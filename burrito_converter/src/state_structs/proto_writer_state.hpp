#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Attribute::Image {
class Image;
}

////////////////////////////////////////////////////////////////////////////////
// ProtoWriterState
//
// A set of variables that are persisted while writing a guildpoint file.
// TODO: This should be renamed to GuildpointWriterState
////////////////////////////////////////////////////////////////////////////////
class ProtoWriterState {
 public:
    ProtoWriterState();

    // The root path of the marker pack for relative file paths.
    // TODO: This should be renamed to marker_pack_root_path because not all roots are directories
    std::string marker_pack_root_directory;

    // A map from texture path to the index within "textures" that the path is saved in.
    std::map<std::string, uint32_t> texture_path_to_textures_index;

    // A list of all of the textures with their paths.
    std::vector<const Attribute::Image::Image*> textures;
};
