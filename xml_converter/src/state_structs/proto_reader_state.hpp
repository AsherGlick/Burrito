#pragma once

#include <string>

#include "waypoint.pb.h"

struct ProtoReaderState {
    // A list of all of the textures with their paths.
    google::protobuf::RepeatedPtrField<::waypoint::TextureData> textures;
    const char* proto_filedir;
};
