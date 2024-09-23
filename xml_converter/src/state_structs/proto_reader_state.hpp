#pragma once

#include <string>

#include "guildpoint.pb.h"

struct ProtoReaderState {
    // A list of all of the textures with their paths.
    google::protobuf::RepeatedPtrField<::guildpoint::TextureData> textures;
    std::string marker_pack_root_directory;
};
