#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
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
MountFilter parse_mount_filter(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);
std::string stringify_mount_filter(MountFilter attribute_value);
waypoint::MountFilter* to_proto_mount_filter(MountFilter attribute_value);
MountFilter from_proto_mount_filter(waypoint::MountFilter proto_mount_filter);
