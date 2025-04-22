#include "proto_writer_state.hpp"

ProtoWriterState::ProtoWriterState() {
    // Push back a "null" first element into the array of textures. We store
    // indexes into this array as unsigned integers, and having the 0th index
    // always be a null value allows us to use 0 to indicate that a texture
    // does not exist in the protocol buffer message. Because 0 is the default
    // value for unsigned integers in proto3 it will be automatically elided
    // from the serialized message. This then guarantees that if a texture
    // is not set for a given message then the texture index will be elided.
    this->textures.push_back(nullptr);
}
