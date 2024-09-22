#include "{{attribute_name}}_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "guildpoint.pb.h"

using namespace std;

void xml_attribute_to_{{attribute_name}}(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    {{class_name}}* value,
    bool* is_set) {
    {{class_name}} {{attribute_name}};
    string normalized_value = normalize(get_attribute_value(input));
    {% for n, attribute_component in enumerate(attribute_components) %}
        {% for i, value in enumerate(attribute_component.xml_fields) %}
            {% if i == 0 and n == 0 %}
                if (normalized_value == "{{value}}") {
                    {{attribute_name}} = {{class_name}}::{{attribute_component.attribute_name}};
                }
            {% else %}
                else if (normalized_value == "{{value}}") {
                    {{attribute_name}} = {{class_name}}::{{attribute_component.attribute_name}};
                }
            {% endif %}
        {% endfor %}
    {% endfor %}
    else {
        errors->push_back(new XMLAttributeValueError("Found an invalid value that was not in the Enum {{class_name}}", input));
        {{attribute_name}} = {{class_name}}::{{attribute_components[0].attribute_name}};
    }
    *value = {{attribute_name}};
    *is_set = true;
}

string {{attribute_name}}_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState*,
    const {{class_name}}* value) {
    {% for n, attribute_component in enumerate(attribute_components) %}
        {% for i, value in enumerate(attribute_component.xml_fields) %}
            {%-if i == 0 and n == 0:%}
                if (*value == {{class_name}}::{{attribute_component.attribute_name}}) {
            {% else %}
                else if (*value == {{class_name}}::{{attribute_component.attribute_name}}) {
            {%  endif %}
                return " " + attribute_name + "=\"" + "{{value}}" + "\"";
            }
        {%  endfor %}
    {%  endfor %}
    else {
        return " " + attribute_name + "=\"" + "{{class_name}}::{{attribute_components[0].xml_fields[0]}}" + "\"";
    }
}

void proto_to_{{attribute_name}}(
    {{proto_field_cpp_type}} input,
    ProtoReaderState*,
    {{class_name}}* value,
    bool* is_set) {
    switch (input) {
        {% for attribute_component in attribute_components %}
            case {{proto_field_cpp_type}}::{{attribute_component.attribute_name}}:
                *value = {{class_name}}::{{attribute_component.attribute_name}};
                *is_set = true;
                break;
        {% endfor %}
        default:
            *value = {{class_name}}::{{attribute_components[0].attribute_name}};
            *is_set = true;
            break;
    }
}

void {{attribute_name}}_to_proto(
    {{class_name}} value,
    ProtoWriterState*,
    std::function<void({{proto_field_cpp_type}})> setter) {
    switch (value) {
        {% for attribute_component in attribute_components %}
            case {{class_name}}::{{attribute_component.attribute_name}}:
                setter({{proto_field_cpp_type}}::{{attribute_component.attribute_name}});
                break;
        {% endfor %}
        default:
            setter({{proto_field_cpp_type}}::{{attribute_components[0].attribute_name}});
            break;
    }
}
