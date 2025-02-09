#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/proto_reader_state.hpp"
#include "../state_structs/proto_writer_state.hpp"
#include "../state_structs/xml_reader_state.hpp"
#include "../state_structs/xml_writer_state.hpp"
#include "../file_helper.hpp"


class XMLError;

class Image {
 public:
    Image();
    Image(std::string base, std::string relative_filepath);
    MarkerPackFile filepath;
};

void xml_attribute_to_image(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    Image* value,
    bool* is_set);

void image_to_xml_attribute(
    XMLWriterState* state,
    const Image* value,
    std::function<void(std::string)> setter);

void proto_to_image(
    unsigned int input,
    ProtoReaderState* state,
    Image* value,
    bool* is_set);

void image_to_proto(
    const Image& value,
    ProtoWriterState* state,
    std::function<void(unsigned int)> setter);
