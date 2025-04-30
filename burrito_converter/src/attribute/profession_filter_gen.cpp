#include "profession_filter_gen.hpp"

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
// Reads a ProfessionFilter from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::ProfessionFilter::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    ProfessionFilter* value,
    bool* is_set
) {
    ProfessionFilter profession_filter;
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");
    profession_filter.guardian = false;
    profession_filter.warrior = false;
    profession_filter.engineer = false;
    profession_filter.ranger = false;
    profession_filter.thief = false;
    profession_filter.elementalist = false;
    profession_filter.mesmer = false;
    profession_filter.necromancer = false;
    profession_filter.revenant = false;

    for (string flag_value : flag_values) {
        string normalized_flag_value = normalize(flag_value);
        if (normalized_flag_value == "guardian") {
            profession_filter.guardian = true;
        }
        else if (normalized_flag_value == "warrior") {
            profession_filter.warrior = true;
        }
        else if (normalized_flag_value == "engineer") {
            profession_filter.engineer = true;
        }
        else if (normalized_flag_value == "ranger") {
            profession_filter.ranger = true;
        }
        else if (normalized_flag_value == "thief") {
            profession_filter.thief = true;
        }
        else if (normalized_flag_value == "elementalist") {
            profession_filter.elementalist = true;
        }
        else if (normalized_flag_value == "mesmer") {
            profession_filter.mesmer = true;
        }
        else if (normalized_flag_value == "necromancer") {
            profession_filter.necromancer = true;
        }
        else if (normalized_flag_value == "revenant") {
            profession_filter.revenant = true;
        }
        else {
            errors->push_back(new XMLAttributeValueError("Invalid Filter for ProfessionFilter. Found " + flag_value, input));
            continue;
        }
    }
    *value = profession_filter;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Writes a ProfessionFilter to an xml attribute using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::ProfessionFilter::to_xml_attribute(
    XMLWriterState*,
    const ProfessionFilter* value,
    std::function<void(std::string)> setter
) {
    vector<string> flag_values;
    if (value->guardian == true) {
        flag_values.push_back("guardian");
    }
    if (value->warrior == true) {
        flag_values.push_back("warrior");
    }
    if (value->engineer == true) {
        flag_values.push_back("engineer");
    }
    if (value->ranger == true) {
        flag_values.push_back("ranger");
    }
    if (value->thief == true) {
        flag_values.push_back("thief");
    }
    if (value->elementalist == true) {
        flag_values.push_back("elementalist");
    }
    if (value->mesmer == true) {
        flag_values.push_back("mesmer");
    }
    if (value->necromancer == true) {
        flag_values.push_back("necromancer");
    }
    if (value->revenant == true) {
        flag_values.push_back("revenant");
    }
    string output = join(flag_values, ",");
    setter(output);
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads a ProfessionFilter from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::ProfessionFilter::from_proto_field(
    guildpoint::ProfessionFilter input,
    ProtoReaderState*,
    ProfessionFilter* value,
    bool* is_set
) {
    ProfessionFilter profession_filter;
    profession_filter.guardian = input.guardian();
    profession_filter.warrior = input.warrior();
    profession_filter.engineer = input.engineer();
    profession_filter.ranger = input.ranger();
    profession_filter.thief = input.thief();
    profession_filter.elementalist = input.elementalist();
    profession_filter.mesmer = input.mesmer();
    profession_filter.necromancer = input.necromancer();
    profession_filter.revenant = input.revenant();
    *value = profession_filter;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes a ProfessionFilter to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::ProfessionFilter::to_proto_field(
    ProfessionFilter value,
    ProtoWriterState*,
    std::function<void(guildpoint::ProfessionFilter*)> setter
) {
    guildpoint::ProfessionFilter* proto_profession_filter = new guildpoint::ProfessionFilter();
    bool should_write = false;
    proto_profession_filter->set_guardian(value.guardian);
    should_write |= value.guardian;
    proto_profession_filter->set_warrior(value.warrior);
    should_write |= value.warrior;
    proto_profession_filter->set_engineer(value.engineer);
    should_write |= value.engineer;
    proto_profession_filter->set_ranger(value.ranger);
    should_write |= value.ranger;
    proto_profession_filter->set_thief(value.thief);
    should_write |= value.thief;
    proto_profession_filter->set_elementalist(value.elementalist);
    should_write |= value.elementalist;
    proto_profession_filter->set_mesmer(value.mesmer);
    should_write |= value.mesmer;
    proto_profession_filter->set_necromancer(value.necromancer);
    should_write |= value.necromancer;
    proto_profession_filter->set_revenant(value.revenant);
    should_write |= value.revenant;
    if (should_write) {
        setter(proto_profession_filter);
    }
}
