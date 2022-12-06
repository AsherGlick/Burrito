#include "{{attribute_name}}_gen.hpp"

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
    string normalized_value = normalize(get_attribute_value(input));
    {% for n, attribute_variable in enumerate(attribute_variables) %}
    {% for i, value in enumerate(attribute_variable.xml_fields) %}
    {% if i == 0 and n == 0: %}
    if (normalized_value == "{{value}}") {
        {{attribute_name}} = {{class_name}}::{{attribute_variable.attribute_name}};
    }
    {% else: %}
    else if (normalized_value == "{{value}}") {
        {{attribute_name}} = {{class_name}}::{{attribute_variable.attribute_name}};
    }
    {% endif %}
    {% endfor %}
    {% endfor %}
    else {
        errors->push_back(new XMLAttributeValueError("Found an invalid value that was not in the Enum {{class_name}}", input));
        {{attribute_name}} = {{class_name}}::{{attribute_variables[0].attribute_name}};
    }
    return {{attribute_name}};
}

string stringify_{{attribute_name}}({{class_name}} attribute_value) {
    {% for n, attribute_variable in enumerate(attribute_variables) %}
    {% for i, value in enumerate(attribute_variable.xml_fields) %}
        {%-if i == 0 and n == 0:%}
    if (attribute_value == {{class_name}}::{{attribute_variable.attribute_name}}) {
        return "{{value}}";
    }
    {% else: %}
    else if (attribute_value == {{class_name}}::{{attribute_variable.attribute_name}}) {
        return "{{value}}";
    }
    {%  endif %}
    {%  endfor %}
    {%  endfor %}
    else {
        return "{{class_name}}::{{attribute_variables[0].xml_fields[0]}}";
    }
}

waypoint::{{class_name}} to_proto_{{attribute_name}}({{class_name}} attribute_value) {
    waypoint::{{class_name}} proto_{{attribute_name}};
    {% for n, attribute_variable in enumerate(attribute_variables) %}
    {% for i, value in enumerate(attribute_variable.xml_fields) %}
    {% if i == 0 and n == 0: %}
    if (attribute_value == {{class_name}}::{{attribute_variable.attribute_name}}) {
        proto_{{attribute_name}} = waypoint::{{class_name}}::{{attribute_variable.attribute_name}};
    }
    {% else: %}
    else if (attribute_value == {{class_name}}::{{attribute_variable.attribute_name}}) {
        proto_{{attribute_name}} = waypoint::{{class_name}}::{{attribute_variable.attribute_name}};
    }
    {% endif %}
    {% endfor %}
    {% endfor %}
    else {
        proto_{{attribute_name}} = waypoint::{{class_name}}::{{attribute_variables[0].attribute_name}};
    }
    return proto_{{attribute_name}};
}

{{class_name}} from_proto_{{attribute_name}}(waypoint::{{class_name}} proto_{{attribute_name}}) {
    {{class_name}} {{attribute_name}};
    {% for n, attribute_variable in enumerate(attribute_variables) %}
    {% for i, value in enumerate(attribute_variable.xml_fields) %}
    {% if i == 0 and n == 0: %}
    if (proto_{{attribute_name}} == waypoint::{{class_name}}::{{attribute_variable.attribute_name}}) {
        {{attribute_name}} = {{class_name}}::{{attribute_variable.attribute_name}};
    }
    {% else: %}
    else if (proto_{{attribute_name}} == waypoint::{{class_name}}::{{attribute_variable.attribute_name}}) {
        {{attribute_name}} = {{class_name}}::{{attribute_variable.attribute_name}};
    }
    {% endif %}
    {% endfor %}
    {% endfor %}
    else {
        {{attribute_name}} = {{class_name}}::{{attribute_variables[0].attribute_name}};
    }
    return {{attribute_name}};
}
