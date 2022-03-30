#pragma once

#include "../rapidxml-1.13/rapidxml.hpp"
#include "filter.hpp"


using namespace std;

class MountFilter: public Filter {
public:
    FILTER_ITEM(jackal, "jackal")
    FILTER_ITEM(griffon, "griffon")
    FILTER_ITEM(springer, "springer")
    FILTER_ITEM(skimmer, "skimmer")
    FILTER_ITEM(raptor, "raptor")
    FILTER_ITEM(rollerbeetle, "rollerbeetle")
    FILTER_ITEM(warclaw, "warclaw")
    FILTER_ITEM(skyscale, "skyscale")
    FILTER_ITEM(skiff, "skiff")
    FILTER_ITEM(seige_turtle, "seigeturtle")


    virtual string classname() { return "MountFilter"; }
};

MountFilter parse_MountFilter(rapidxml::xml_attribute<>* input, vector<string> *errors);
