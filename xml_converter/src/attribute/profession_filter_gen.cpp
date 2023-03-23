#include "profession_filter_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "waypoint.pb.h"

using namespace std;

ProfessionFilter parse_profession_filter(rapidxml::xml_attribute<>* input, vector<XMLError*>* errors) {
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
    return profession_filter;
}

string stringify_profession_filter(ProfessionFilter attribute_value) {
    string output = "";
    if (attribute_value.guardian == true) {
        output = output + "guardian";
    }
    if (attribute_value.warrior == true) {
        output = output + "warrior";
    }
    if (attribute_value.engineer == true) {
        output = output + "engineer";
    }
    if (attribute_value.ranger == true) {
        output = output + "ranger";
    }
    if (attribute_value.thief == true) {
        output = output + "thief";
    }
    if (attribute_value.elementalist == true) {
        output = output + "elementalist";
    }
    if (attribute_value.mesmer == true) {
        output = output + "mesmer";
    }
    if (attribute_value.necromancer == true) {
        output = output + "necromancer";
    }
    if (attribute_value.revenant == true) {
        output = output + "revenant";
    }
    return output;
}

waypoint::ProfessionFilter* to_proto_profession_filter(ProfessionFilter attribute_value) {
    waypoint::ProfessionFilter* proto_profession_filter = new waypoint::ProfessionFilter();
    proto_profession_filter->set_guardian(attribute_value.guardian);
    proto_profession_filter->set_warrior(attribute_value.warrior);
    proto_profession_filter->set_engineer(attribute_value.engineer);
    proto_profession_filter->set_ranger(attribute_value.ranger);
    proto_profession_filter->set_thief(attribute_value.thief);
    proto_profession_filter->set_elementalist(attribute_value.elementalist);
    proto_profession_filter->set_mesmer(attribute_value.mesmer);
    proto_profession_filter->set_necromancer(attribute_value.necromancer);
    proto_profession_filter->set_revenant(attribute_value.revenant);
    return proto_profession_filter;
}

ProfessionFilter from_proto_profession_filter(waypoint::ProfessionFilter proto_profession_filter) {
    ProfessionFilter profession_filter;
    profession_filter.guardian = proto_profession_filter.guardian();
    profession_filter.warrior = proto_profession_filter.warrior();
    profession_filter.engineer = proto_profession_filter.engineer();
    profession_filter.ranger = proto_profession_filter.ranger();
    profession_filter.thief = proto_profession_filter.thief();
    profession_filter.elementalist = proto_profession_filter.elementalist();
    profession_filter.mesmer = proto_profession_filter.mesmer();
    profession_filter.necromancer = proto_profession_filter.necromancer();
    profession_filter.revenant = proto_profession_filter.revenant();
    return profession_filter;
}
