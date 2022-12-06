#include "mount_filter_gen.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "waypoint.pb.h"

using namespace std;

MountFilter parse_mount_filter(rapidxml::xml_attribute<>* input, vector<XMLError*>* errors) {
    MountFilter mount_filter;
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");
    mount_filter.raptor = false;
    mount_filter.springer = false;
    mount_filter.skimmer = false;
    mount_filter.jackal = false;
    mount_filter.griffon = false;
    mount_filter.roller_beetle = false;
    mount_filter.warclaw = false;
    mount_filter.skyscale = false;
    mount_filter.skiff = false;
    mount_filter.seige_turtle = false;

    for (string flag_value : flag_values) {
        string normalized_flag_value = normalize(flag_value);
        if (normalized_flag_value == "raptor") {
            mount_filter.raptor = true;
        }
        else if (normalized_flag_value == "springer") {
            mount_filter.springer = true;
        }
        else if (normalized_flag_value == "skimmer") {
            mount_filter.skimmer = true;
        }
        else if (normalized_flag_value == "jackal") {
            mount_filter.jackal = true;
        }
        else if (normalized_flag_value == "griffon") {
            mount_filter.griffon = true;
        }
        else if (normalized_flag_value == "rollerbeetle") {
            mount_filter.roller_beetle = true;
        }
        else if (normalized_flag_value == "warclaw") {
            mount_filter.warclaw = true;
        }
        else if (normalized_flag_value == "skyscale") {
            mount_filter.skyscale = true;
        }
        else if (normalized_flag_value == "skiff") {
            mount_filter.skiff = true;
        }
        else if (normalized_flag_value == "seigeturtle") {
            mount_filter.seige_turtle = true;
        }
        else {
            errors->push_back(new XMLAttributeValueError("Invalid Filter for MountFilter. Found " + flag_value, input));
            continue;
        }
    }
    return mount_filter;
}

string stringify_mount_filter(MountFilter attribute_value) {
    string output = "";
    if (attribute_value.raptor == true) {
        output = output + "raptor";
    }
    if (attribute_value.springer == true) {
        output = output + "springer";
    }
    if (attribute_value.skimmer == true) {
        output = output + "skimmer";
    }
    if (attribute_value.jackal == true) {
        output = output + "jackal";
    }
    if (attribute_value.griffon == true) {
        output = output + "griffon";
    }
    if (attribute_value.roller_beetle == true) {
        output = output + "rollerbeetle";
    }
    if (attribute_value.warclaw == true) {
        output = output + "warclaw";
    }
    if (attribute_value.skyscale == true) {
        output = output + "skyscale";
    }
    if (attribute_value.skiff == true) {
        output = output + "skiff";
    }
    if (attribute_value.seige_turtle == true) {
        output = output + "seigeturtle";
    }
    return output;
}

waypoint::MountFilter* to_proto_mount_filter(MountFilter attribute_value) {
    waypoint::MountFilter* proto_mount_filter = new waypoint::MountFilter();
    if (attribute_value.raptor == true) {
        proto_mount_filter->set_raptor(true);
    }
    if (attribute_value.springer == true) {
        proto_mount_filter->set_springer(true);
    }
    if (attribute_value.skimmer == true) {
        proto_mount_filter->set_skimmer(true);
    }
    if (attribute_value.jackal == true) {
        proto_mount_filter->set_jackal(true);
    }
    if (attribute_value.griffon == true) {
        proto_mount_filter->set_griffon(true);
    }
    if (attribute_value.roller_beetle == true) {
        proto_mount_filter->set_roller_beetle(true);
    }
    if (attribute_value.warclaw == true) {
        proto_mount_filter->set_warclaw(true);
    }
    if (attribute_value.skyscale == true) {
        proto_mount_filter->set_skyscale(true);
    }
    if (attribute_value.skiff == true) {
        proto_mount_filter->set_skiff(true);
    }
    if (attribute_value.seige_turtle == true) {
        proto_mount_filter->set_seige_turtle(true);
    }
    return proto_mount_filter;
}

MountFilter from_proto_mount_filter(waypoint::MountFilter proto_mount_filter) {
    MountFilter mount_filter;
    if (proto_mount_filter.raptor() == true) {
        mount_filter.raptor = true;
    }
    if (proto_mount_filter.springer() == true) {
        mount_filter.springer = true;
    }
    if (proto_mount_filter.skimmer() == true) {
        mount_filter.skimmer = true;
    }
    if (proto_mount_filter.jackal() == true) {
        mount_filter.jackal = true;
    }
    if (proto_mount_filter.griffon() == true) {
        mount_filter.griffon = true;
    }
    if (proto_mount_filter.roller_beetle() == true) {
        mount_filter.roller_beetle = true;
    }
    if (proto_mount_filter.warclaw() == true) {
        mount_filter.warclaw = true;
    }
    if (proto_mount_filter.skyscale() == true) {
        mount_filter.skyscale = true;
    }
    if (proto_mount_filter.skiff() == true) {
        mount_filter.skiff = true;
    }
    if (proto_mount_filter.seige_turtle() == true) {
        mount_filter.seige_turtle = true;
    }
    return mount_filter;
}