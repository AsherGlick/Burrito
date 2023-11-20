#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/xml_reader_state.hpp"

class XMLError;
namespace waypoint {
class MountFilter;
}

class MountFilter {
 public:
    bool griffon;
    bool jackal;
    bool raptor;
    bool roller_beetle;
    bool seige_turtle;
    bool skiff;
    bool skimmer;
    bool skyscale;
    bool springer;
    bool warclaw;

    virtual std::string classname() {
        return "MountFilter";
    }
};
void xml_attribute_to_mount_filter(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    MountFilter* value,
    bool* is_set);

std::string mount_filter_to_xml_attribute(const std::string& attribute_name, const MountFilter* value);

void proto_to_mount_filter(waypoint::MountFilter input, MountFilter* value, bool* is_set);

void mount_filter_to_proto(MountFilter value, std::function<void(waypoint::MountFilter*)> setter);
