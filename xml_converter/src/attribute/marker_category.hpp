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

namespace guildpoint {
class Category;
}

class MarkerCategory {
 public:
    std::string category;
};

void xml_attribute_to_marker_category(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    MarkerCategory* value,
    bool* is_set);

std::string marker_category_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const MarkerCategory* value);

void proto_to_marker_category(
    guildpoint::Category input,
    ProtoReaderState* state,
    MarkerCategory* value,
    bool* is_set);

void marker_category_to_proto(
    MarkerCategory value,
    ProtoWriterState* state,
    std::function<void(guildpoint::Category*)> setter);
