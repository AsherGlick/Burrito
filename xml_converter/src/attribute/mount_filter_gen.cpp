#include "mount_filter_gen.hpp"

#include <algorithm>
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
    proto_mount_filter->set_raptor(attribute_value.raptor);
    proto_mount_filter->set_springer(attribute_value.springer);
    proto_mount_filter->set_skimmer(attribute_value.skimmer);
    proto_mount_filter->set_jackal(attribute_value.jackal);
    proto_mount_filter->set_griffon(attribute_value.griffon);
    proto_mount_filter->set_roller_beetle(attribute_value.roller_beetle);
    proto_mount_filter->set_warclaw(attribute_value.warclaw);
    proto_mount_filter->set_skyscale(attribute_value.skyscale);
    proto_mount_filter->set_skiff(attribute_value.skiff);
    proto_mount_filter->set_seige_turtle(attribute_value.seige_turtle);
    return proto_mount_filter;
}

MountFilter from_proto_mount_filter(waypoint::MountFilter proto_mount_filter) {
    MountFilter mount_filter;
    mount_filter.raptor = proto_mount_filter.raptor();
    mount_filter.springer = proto_mount_filter.springer();
    mount_filter.skimmer = proto_mount_filter.skimmer();
    mount_filter.jackal = proto_mount_filter.jackal();
    mount_filter.griffon = proto_mount_filter.griffon();
    mount_filter.roller_beetle = proto_mount_filter.roller_beetle();
    mount_filter.warclaw = proto_mount_filter.warclaw();
    mount_filter.skyscale = proto_mount_filter.skyscale();
    mount_filter.skiff = proto_mount_filter.skiff();
    mount_filter.seige_turtle = proto_mount_filter.seige_turtle();
    return mount_filter;
}
