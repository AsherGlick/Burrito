#include "species_filter_gen.hpp"

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
// Reads a SpeciesFilter from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::SpeciesFilter::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    SpeciesFilter* value,
    bool* is_set
) {
    SpeciesFilter species_filter;
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");
    species_filter.asura = false;
    species_filter.charr = false;
    species_filter.human = false;
    species_filter.norn = false;
    species_filter.sylvari = false;

    for (string flag_value : flag_values) {
        string normalized_flag_value = normalize(flag_value);
        if (normalized_flag_value == "asura") {
            species_filter.asura = true;
        }
        else if (normalized_flag_value == "charr") {
            species_filter.charr = true;
        }
        else if (normalized_flag_value == "human") {
            species_filter.human = true;
        }
        else if (normalized_flag_value == "norn") {
            species_filter.norn = true;
        }
        else if (normalized_flag_value == "sylvari") {
            species_filter.sylvari = true;
        }
        else {
            errors->push_back(new XMLAttributeValueError("Invalid Filter for SpeciesFilter. Found " + flag_value, input));
            continue;
        }
    }
    *value = species_filter;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Writes a SpeciesFilter to an xml attribute using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::SpeciesFilter::to_xml_attribute(
    XMLWriterState*,
    const SpeciesFilter* value,
    std::function<void(std::string)> setter
) {
    vector<string> flag_values;
    if (value->asura == true) {
        flag_values.push_back("asura");
    }
    if (value->charr == true) {
        flag_values.push_back("charr");
    }
    if (value->human == true) {
        flag_values.push_back("human");
    }
    if (value->norn == true) {
        flag_values.push_back("norn");
    }
    if (value->sylvari == true) {
        flag_values.push_back("sylvari");
    }
    string output = join(flag_values, ",");
    setter(output);
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads a SpeciesFilter from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::SpeciesFilter::from_proto_field(
    guildpoint::SpeciesFilter input,
    ProtoReaderState*,
    SpeciesFilter* value,
    bool* is_set
) {
    SpeciesFilter species_filter;
    species_filter.asura = input.asura();
    species_filter.charr = input.charr();
    species_filter.human = input.human();
    species_filter.norn = input.norn();
    species_filter.sylvari = input.sylvari();
    *value = species_filter;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes a SpeciesFilter to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::SpeciesFilter::to_proto_field(
    SpeciesFilter value,
    ProtoWriterState*,
    std::function<void(guildpoint::SpeciesFilter*)> setter
) {
    guildpoint::SpeciesFilter* proto_species_filter = new guildpoint::SpeciesFilter();
    bool should_write = false;
    proto_species_filter->set_asura(value.asura);
    should_write |= value.asura;
    proto_species_filter->set_charr(value.charr);
    should_write |= value.charr;
    proto_species_filter->set_human(value.human);
    should_write |= value.human;
    proto_species_filter->set_norn(value.norn);
    should_write |= value.norn;
    proto_species_filter->set_sylvari(value.sylvari);
    should_write |= value.sylvari;
    if (should_write) {
        setter(proto_species_filter);
    }
}
