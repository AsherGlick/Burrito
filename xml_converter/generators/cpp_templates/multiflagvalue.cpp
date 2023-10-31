#include "{{attribute_name}}_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "waypoint.pb.h"

using namespace std;

{{class_name}} parse_{{attribute_name}}(rapidxml::xml_attribute<>* input, vector<XMLError*>* errors) {
    {{class_name}} {{attribute_name}};
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");
    {% for attribute_variable in attribute_variables %}
        {{attribute_name}}.{{attribute_variable.attribute_name}} = false;
    {% endfor %}

    for (string flag_value : flag_values) {
        string normalized_flag_value = normalize(flag_value);
        {% for n, attribute_variable in enumerate(attribute_variables) %}
            {% for i, value in enumerate(attribute_variable.xml_fields) %}
                {% if i == 0 and n == 0: %}
                    if (normalized_flag_value == "{{value}}") {
                        {{attribute_name}}.{{attribute_variable.attribute_name}} = true;
                    }
                {% else: %}
                    else if (normalized_flag_value == "{{value}}") {
                        {{attribute_name}}.{{attribute_variable.attribute_name}} = true;
                    }
                {% endif %}
            {%- endfor %}
        {%- endfor %}
        else {
            errors->push_back(new XMLAttributeValueError("Invalid Filter for {{class_name}}. Found " + flag_value, input));
            continue;
        }
    }
    return {{attribute_name}};
}

string stringify_{{attribute_name}}({{class_name}} attribute_value) {
    vector<string> flag_values;
    {% for n, attribute_variable in enumerate(attribute_variables)%}
        if (attribute_value.{{attribute_variable.attribute_name}} == true) {
            flag_values.push_back("{{attribute_variable.xml_fields[0]}}");
        }
    {% endfor %}
    string output = "";
    for (size_t i = 0; i < flag_values.size(); ++i) {
        output += flag_values[i];
        if (i < flag_values.size() - 1) {
            output += ",";
        }
    }
    return output;
}

waypoint::{{class_name}}* to_proto_{{attribute_name}}({{class_name}} attribute_value) {
    waypoint::{{class_name}}* proto_{{attribute_name}} = new waypoint::{{class_name}}();
    {% for n, attribute_variable in enumerate(attribute_variables)%}
        proto_{{attribute_name}}->set_{{attribute_variable.attribute_name}}(attribute_value.{{attribute_variable.attribute_name}});
    {% endfor %}
    return proto_{{attribute_name}};
}

{{class_name}} from_proto_{{attribute_name}}(waypoint::{{class_name}} proto_{{attribute_name}}) {
    {{class_name}} {{attribute_name}};
    {% for n, attribute_variable in enumerate(attribute_variables)%}
        {{attribute_name}}.{{attribute_variable.attribute_name}} = proto_{{attribute_name}}.{{attribute_variable.attribute_name}}();
    {% endfor %}
    return {{attribute_name}};
}
