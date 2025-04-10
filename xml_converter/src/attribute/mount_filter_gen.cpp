#include "mount_filter_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "guildpoint.pb.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads a MountFilter from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::MountFilter::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    MountFilter* value,
    bool* is_set
) {
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
    *value = mount_filter;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Writes a MountFilter to an xml attribute using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::MountFilter::to_xml_attribute(
    XMLWriterState*,
    const MountFilter* value,
    std::function<void(std::string)> setter
) {
    vector<string> flag_values;
    if (value->raptor == true) {
        flag_values.push_back("raptor");
    }
    if (value->springer == true) {
        flag_values.push_back("springer");
    }
    if (value->skimmer == true) {
        flag_values.push_back("skimmer");
    }
    if (value->jackal == true) {
        flag_values.push_back("jackal");
    }
    if (value->griffon == true) {
        flag_values.push_back("griffon");
    }
    if (value->roller_beetle == true) {
        flag_values.push_back("rollerbeetle");
    }
    if (value->warclaw == true) {
        flag_values.push_back("warclaw");
    }
    if (value->skyscale == true) {
        flag_values.push_back("skyscale");
    }
    if (value->skiff == true) {
        flag_values.push_back("skiff");
    }
    if (value->seige_turtle == true) {
        flag_values.push_back("seigeturtle");
    }
    string output = join(flag_values, ",");
    setter(output);
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads a MountFilter from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::MountFilter::from_proto_field(
    guildpoint::MountFilter input,
    ProtoReaderState*,
    MountFilter* value,
    bool* is_set
) {
    MountFilter mount_filter;
    mount_filter.raptor = input.raptor();
    mount_filter.springer = input.springer();
    mount_filter.skimmer = input.skimmer();
    mount_filter.jackal = input.jackal();
    mount_filter.griffon = input.griffon();
    mount_filter.roller_beetle = input.roller_beetle();
    mount_filter.warclaw = input.warclaw();
    mount_filter.skyscale = input.skyscale();
    mount_filter.skiff = input.skiff();
    mount_filter.seige_turtle = input.seige_turtle();
    *value = mount_filter;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes a MountFilter to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::MountFilter::to_proto_field(
    MountFilter value,
    ProtoWriterState*,
    std::function<void(guildpoint::MountFilter*)> setter
) {
    guildpoint::MountFilter* proto_mount_filter = new guildpoint::MountFilter();
    bool should_write = false;
    proto_mount_filter->set_raptor(value.raptor);
    should_write |= value.raptor;
    proto_mount_filter->set_springer(value.springer);
    should_write |= value.springer;
    proto_mount_filter->set_skimmer(value.skimmer);
    should_write |= value.skimmer;
    proto_mount_filter->set_jackal(value.jackal);
    should_write |= value.jackal;
    proto_mount_filter->set_griffon(value.griffon);
    should_write |= value.griffon;
    proto_mount_filter->set_roller_beetle(value.roller_beetle);
    should_write |= value.roller_beetle;
    proto_mount_filter->set_warclaw(value.warclaw);
    should_write |= value.warclaw;
    proto_mount_filter->set_skyscale(value.skyscale);
    should_write |= value.skyscale;
    proto_mount_filter->set_skiff(value.skiff);
    should_write |= value.skiff;
    proto_mount_filter->set_seige_turtle(value.seige_turtle);
    should_write |= value.seige_turtle;
    if (should_write) {
        setter(proto_mount_filter);
    }
}
