#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

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
    };
};
MountFilter parse_mount_filter(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);
std::string stringify_mount_filter(MountFilter attribute_value);
