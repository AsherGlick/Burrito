#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
{% if type == "Enum":%}
    #include "waypoint.pb.h"

    class XMLError;

    enum {{class_name}} {
        {% for attribute_variable in attribute_variables: %}
        {{attribute_variable.attribute_name}},
        {% endfor %}
    };
{% else: %}
    class XMLError;
    namespace waypoint {
    class {{class_name}};
    }

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
{% if type == "Enum":%}
    waypoint::{{class_name}} to_proto_{{attribute_name}}({{class_name}} attribute_value);
{% else: %}
    waypoint::{{class_name}}* to_proto_{{attribute_name}}({{class_name}} attribute_value);
{% endif %}
{{class_name}} from_proto_{{attribute_name}}(waypoint::{{class_name}} proto_{{attribute_name}});
