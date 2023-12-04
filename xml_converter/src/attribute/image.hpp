#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/proto_reader_state.hpp"
#include "../state_structs/proto_writer_state.hpp"
#include "../state_structs/xml_reader_state.hpp"
#include "../state_structs/xml_writer_state.hpp"

class XMLError;

namespace waypoint {
class TexturePath;
}

class Image {
 public:
    std::string path;
};

void xml_attribute_to_image(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    Image* value,
    bool* is_set);

std::string image_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const Image* value);

void proto_to_image(
    waypoint::TexturePath input,
    ProtoReaderState* state,
    Image* value,
    bool* is_set);

void image_to_proto(
    Image value,
    ProtoWriterState* state,
    std::function<void(waypoint::TexturePath*)> setter);
