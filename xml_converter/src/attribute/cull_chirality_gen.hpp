#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/proto_reader_state.hpp"
#include "../state_structs/proto_writer_state.hpp"
#include "../state_structs/xml_reader_state.hpp"
#include "../state_structs/xml_writer_state.hpp"
#include "guildpoint.pb.h"

class XMLError;

namespace Attribute::CullChirality {

enum CullChirality {
    clockwise,
    counter_clockwise,
    none,
};
void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    CullChirality* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const CullChirality* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    guildpoint::CullChirality input,
    ProtoReaderState* state,
    CullChirality* value,
    bool* is_set
);

void to_proto_field(
    CullChirality value,
    ProtoWriterState* state,
    std::function<void(guildpoint::CullChirality)> setter
);

}  // namespace Attribute::CullChirality
