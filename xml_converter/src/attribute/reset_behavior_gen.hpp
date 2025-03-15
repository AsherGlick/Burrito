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

namespace Attribute::ResetBehavior {

enum ResetBehavior {
    always_visible,
    daily_reset,
    daily_reset_per_character,
    instance_change,
    map_change,
    map_reset,
    never,
    timer,
    weekly_reset,
};
void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    ResetBehavior* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const ResetBehavior* value,
    std::function<void(std::string)> setter
);

}  // namespace Attribute::ResetBehavior
