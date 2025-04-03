#include "proto_writer_state.hpp"

ProtoWriterState::ProtoWriterState() {
    // Push back a "null" first element indicating "no texture" that can be accessed with index 0.
    this->textures.push_back(nullptr);
}
