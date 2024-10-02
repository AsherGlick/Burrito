#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/proto_reader_state.hpp"
#include "../state_structs/proto_writer_state.hpp"
#include "../state_structs/xml_reader_state.hpp"
#include "../state_structs/xml_writer_state.hpp"
{% if type == "Enum" %}
    #include "guildpoint.pb.h"

    class XMLError;

    enum {{class_name}} {
        {% for attribute_component in attribute_components %}
            {{attribute_component.attribute_name}},
        {% endfor %}
    };
{% elif type in ["CompoundValue", "MultiflagValue"] %}

    class XMLError;
    {{proto_field_cpp_type_prototype}}

    class {{class_name}} {
     public:
        {% for attribute_component in attribute_components %}
            {{attribute_component.cpp_type}} {{attribute_component.attribute_name}};
        {% endfor %}

        virtual std::string classname() {
            return "{{class_name}}";
        }
    };
{% endif %}
void xml_attribute_to_{{attribute_name}}(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    {{class_name}}* value,
    bool* is_set);

std::string {{attribute_name}}_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState* state,
    const {{class_name}}* value);

void proto_to_{{attribute_name}}(
    {{proto_field_cpp_type}} input,
    ProtoReaderState* state,
    {{class_name}}* value,
    bool* is_set);

{% if type == "Enum" %}
    void {{attribute_name}}_to_proto(
        {{class_name}} value,
        ProtoWriterState* state,
        std::function<void({{proto_field_cpp_type}})> setter);
{% else %}
    void {{attribute_name}}_to_proto(
        {{class_name}} value,
        ProtoWriterState* state,
        std::function<void({{proto_field_cpp_type}}*)> setter);
{% endif %}