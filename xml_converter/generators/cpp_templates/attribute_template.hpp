#pragma once

#include <string>
#include <vector>
#include <functional>

#include "../rapidxml-1.13/rapidxml.hpp"
{% if type == "Enum" %}
    #include "waypoint.pb.h"

    class XMLError;

    enum {{class_name}} {
        {% for attribute_variable in attribute_variables: %}
        {{attribute_variable.attribute_name}},
        {% endfor %}
    };
{% else %}
    class XMLError;
    {{proto_field_cpp_type_prototype}}

    class {{class_name}} {
     public:
        {% for attribute_variable in attribute_variables: %}
        {{attribute_variable.cpp_type}} {{attribute_variable.attribute_name}};
        {% endfor %}

        virtual std::string classname() {
            return "{{class_name}}";
        }
    };
{% endif %}
void xml_attribute_to_{{attribute_name}}(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    {{class_name}}* value,
    bool* is_set);
std::string {{attribute_name}}_to_xml_attribute(const std::string& attribute_name, const {{class_name}}* value);

{{class_name}} from_proto_{{attribute_name}}({{proto_field_cpp_type}} proto_{{attribute_name}});

{% if type == "Enum" %}
    void {{attribute_name}}_to_proto({{class_name}} value, std::function<void({{proto_field_cpp_type}})> setter);
{% else %}
    void {{attribute_name}}_to_proto({{class_name}} value, std::function<void({{proto_field_cpp_type}}*)> setter);
{% endif %}