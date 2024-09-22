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
class ProfessionFilter;
}

class ProfessionFilter {
 public:
    bool elementalist;
    bool engineer;
    bool guardian;
    bool mesmer;
    bool necromancer;
    bool ranger;
    bool revenant;
    bool thief;
    bool warrior;

    virtual std::string classname() {
        return "ProfessionFilter";
    }
};
void xml_attribute_to_profession_filter(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    ProfessionFilter* value,
    bool* is_set);

std::string profession_filter_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const ProfessionFilter* value);

void proto_to_profession_filter(
    guildpoint::ProfessionFilter input,
    ProtoReaderState* state,
    ProfessionFilter* value,
    bool* is_set);

void profession_filter_to_proto(
    ProfessionFilter value,
    ProtoWriterState* state,
    std::function<void(guildpoint::ProfessionFilter*)> setter);
