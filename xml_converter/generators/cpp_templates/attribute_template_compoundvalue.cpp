#include "{{attribute_name}}_gen.hpp"

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
// Reads a {{class_name}} from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::{{namespace}}::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>*,
    XMLReaderState*,
    {{class_name}}* value,
    bool* is_set
) {
    {{class_name}} {{attribute_name}};
    vector<string> compound_values;
    string attributename;
    {% for attribute_component in attribute_components %}
        {{attribute_name}}.{{attribute_component.attribute_name}} = 0;
    {% endfor %}
    attributename = get_attribute_name(input);
    compound_values = split(get_attribute_value(input), ",");
    if (compound_values.size() == {{ attribute_components|length }}) {
        {% for n, attribute_component in enumerate(attribute_components) %}
            {{attribute_name}}.{{attribute_component.attribute_name}} = std::stof(compound_values[{{n}}]);
        {% endfor %}
    }
    *value = {{attribute_name}};
    *is_set = true;
}
{% if xml_bundled_components != [] %}

    ////////////////////////////////////////////////////////////////////////////////
    // to_xml_attribute
    //
    // Writes a {{class_name}} to an xml attribute using the provided setter function.
    ////////////////////////////////////////////////////////////////////////////////
    void Attribute::{{namespace}}::to_xml_attribute(
        XMLWriterState*,
        const {{class_name}}* value,
        std::function<void(std::string)> setter
    ) {
        string output;
        {% for n, attribute_component in enumerate(attribute_components) %}
            {% if attribute_component.attribute_name in xml_bundled_components %}
                {% if n == 0 %}
                    output = to_string(value->{{attribute_component.attribute_name}});
                {% else %}
                    output = output + "," + to_string(value->{{attribute_component.attribute_name}});
                {% endif %}
            {% endif %}
        {% endfor %}
        setter(output);
    }
{% endif %}
{% if exclude_from_protobuf == false %}

    ////////////////////////////////////////////////////////////////////////////////
    // from_proto_field
    //
    // Reads a {{class_name}} from a proto field.
    ////////////////////////////////////////////////////////////////////////////////
    void Attribute::{{namespace}}::from_proto_field(
        {{proto_field_cpp_type}} input,
        ProtoReaderState*,
        {{class_name}}* value,
        bool* is_set
    ) {
        {{class_name}} {{attribute_name}};
        {% for attribute_component in attribute_components %}
            {% if attribute_component.protobuf_field != None %}
                {{attribute_name}}.{{attribute_component.attribute_name}} = input.{{attribute_component.protobuf_field}}();
            {% endif %}
        {% endfor %}
        *value = {{attribute_name}};
        *is_set = true;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // to_proto_field
    //
    // Writes a {{class_name}} to a proto using the provided setter function.
    ////////////////////////////////////////////////////////////////////////////////
    void Attribute::{{namespace}}::to_proto_field(
        {{class_name}} value,
        ProtoWriterState*,
        std::function<void({{proto_field_cpp_type}}*)> setter
    ) {
        {{proto_field_cpp_type}}* proto_{{attribute_name}} = new {{proto_field_cpp_type}}();
        {% for attribute_component in attribute_components %}
            {% if attribute_component.protobuf_field != None %}
                proto_{{attribute_name}}->set_{{attribute_component.protobuf_field}}(value.{{attribute_component.attribute_name}});
            {% endif %}
        {% endfor %}
        setter(proto_{{attribute_name}});
    }
{% endif %}
