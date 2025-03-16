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

namespace Attribute::Bounce {

enum Bounce {
    bounce,
    rise,
};
void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    Bounce* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const Bounce* value,
    std::function<void(std::string)> setter
);

}  // namespace Attribute::Bounce
