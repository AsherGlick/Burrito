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

void xml_attribute_to_{{attribute_name}}(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    {{class_name}}* value,
    bool* is_set) {
    {{class_name}} {{attribute_name}};
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");
    {% for attribute_component in attribute_components %}
        {{attribute_name}}.{{attribute_component.attribute_name}} = false;
    {% endfor %}

    for (string flag_value : flag_values) {
        string normalized_flag_value = normalize(flag_value);
        {% for n, attribute_component in enumerate(attribute_components) %}
            {% for i, value in enumerate(attribute_component.xml_fields) %}
                {% if i == 0 and n == 0 %}
                    if (normalized_flag_value == "{{value}}") {
                        {{attribute_name}}.{{attribute_component.attribute_name}} = true;
                    }
                {% else %}
                    else if (normalized_flag_value == "{{value}}") {
                        {{attribute_name}}.{{attribute_component.attribute_name}} = true;
                    }
                {% endif %}
            {%- endfor %}
        {%- endfor %}
        else {
            errors->push_back(new XMLAttributeValueError("Invalid Filter for {{class_name}}. Found " + flag_value, input));
            continue;
        }
    }
    *value = {{attribute_name}};
    *is_set = true;
}

string {{attribute_name}}_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const {{class_name}}* value) {
    vector<string> flag_values;
    {% for n, attribute_component in enumerate(attribute_components) %}
        if (value->{{attribute_component.attribute_name}} == true) {
            flag_values.push_back("{{attribute_component.xml_fields[0]}}");
        }
    {% endfor %}
    string output = join(flag_values, ",");
    return " " + attribute_name + "=\"" + output + "\"";
}

void proto_to_{{attribute_name}}(
    {{proto_field_cpp_type}} input,
    ProtoReaderState* state,
    {{class_name}}* value,
    bool* is_set) {
    {{class_name}} {{attribute_name}};
    {% for n, attribute_component in enumerate(attribute_components) %}
        {{attribute_name}}.{{attribute_component.attribute_name}} = input.{{attribute_component.attribute_name}}();
    {% endfor %}
    *value = {{attribute_name}};
    *is_set = true;
}

void {{attribute_name}}_to_proto(
    {{class_name}} value,
    ProtoWriterState* state,
    std::function<void({{proto_field_cpp_type}}*)> setter) {
    {{proto_field_cpp_type}}* proto_{{attribute_name}} = new {{proto_field_cpp_type}}();
    bool should_write = false;
    {% for n, attribute_component in enumerate(attribute_components) %}
        proto_{{attribute_name}}->set_{{attribute_component.attribute_name}}(value.{{attribute_component.attribute_name}});
        should_write |= value.{{attribute_component.attribute_name}};
    {% endfor %}
    if (should_write) {
        setter(proto_{{attribute_name}});
    }
}
