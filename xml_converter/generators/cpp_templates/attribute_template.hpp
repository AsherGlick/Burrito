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
{% elif type in ["CompoundValue", "MultiflagValue"] %}

    class XMLError;
    {{proto_field_cpp_type_prototype}}
{% endif %}

namespace Attribute::{{namespace}} {

{% if type == "Enum" %}
    enum {{class_name}} {
        {% for attribute_component in attribute_components %}
            {{attribute_component.attribute_name}},
        {% endfor %}
    };
{% elif type in ["CompoundValue", "MultiflagValue"] %}
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
void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    {{class_name}}* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const {{class_name}}* value,
    std::function<void(std::string)> setter
);
{% if exclude_from_protobuf == false %}

    void from_proto_field(
        {{proto_field_cpp_type}} input,
        ProtoReaderState* state,
        {{class_name}}* value,
        bool* is_set
    );

    {% if type == "Enum" %}
        void to_proto_field(
            {{class_name}} value,
            ProtoWriterState* state,
            std::function<void({{proto_field_cpp_type}})> setter
        );
    {% else %}
        void to_proto_field(
            {{class_name}} value,
            ProtoWriterState* state,
            std::function<void({{proto_field_cpp_type}}*)> setter
        );
    {% endif %}
{% endif %}

}  // namespace Attribute::{{namespace}}
