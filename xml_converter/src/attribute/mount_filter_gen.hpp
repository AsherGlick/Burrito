#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;
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

    virtual string classname() { return "MountFilter"; };
};
MountFilter parse_mount_filter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
string stringify_mount_filter(MountFilter attribute_value);