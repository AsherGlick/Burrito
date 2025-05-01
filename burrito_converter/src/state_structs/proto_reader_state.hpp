#pragma once

#include <string>

#include "guildpoint.pb.h"

////////////////////////////////////////////////////////////////////////////////
// ProtoReaderState
//
// A set of variables that are persisted while reading a guildpoint file.
// TODO: This should be renamed to GuildpointReaderState
////////////////////////////////////////////////////////////////////////////////
struct ProtoReaderState {
    // The root path of the marker pack for relative file paths.
    // TODO: This should be renamed to marker_pack_root_path because not all roots are directories
    std::string marker_pack_root_directory;

    // A list of all of the textures with their paths.
    google::protobuf::RepeatedPtrField<::guildpoint::TextureData> textures;
};
