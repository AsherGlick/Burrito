#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../file_helper.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/proto_reader_state.hpp"
#include "../state_structs/proto_writer_state.hpp"
#include "../state_structs/xml_reader_state.hpp"
#include "../state_structs/xml_writer_state.hpp"

class XMLError;

namespace Attribute::Image {
class Image {
 public:
    Image();
    Image(std::string base, std::string relative_filepath);
    MarkerPackFile filepath;
};

void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    Image* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const Image* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    unsigned int input,
    ProtoReaderState* state,
    Image* value,
    bool* is_set
);

void to_proto_field(
    const Image& value,
    ProtoWriterState* state,
    std::function<void(unsigned int)> setter
);

}  // namespace Attribute::Image
