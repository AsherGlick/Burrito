#include "{{attribute_name}}_gen.hpp"

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
    std::vector<XMLError*>*,
    XMLReaderState*,
    {{class_name}}* value,
    bool* is_set) {
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
    string {{attribute_name}}_to_xml_attribute(
        const std::string& attribute_name,
        XMLWriterState*,
        const {{class_name}}* value) {
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
        return " " + attribute_name + "=\"" + output + "\"";
    }
{% endif %}

void proto_to_{{attribute_name}}(
    {{proto_field_cpp_type}} input,
    ProtoReaderState*,
    {{class_name}}* value,
    bool* is_set) {
    {{class_name}} {{attribute_name}};
    {% for attribute_component in attribute_components %}
        {{attribute_name}}.{{attribute_component.attribute_name}} = input.{{attribute_component.protobuf_field}}();
    {% endfor %}
    *value = {{attribute_name}};
    *is_set = true;
}

void {{attribute_name}}_to_proto(
    {{class_name}} value,
    ProtoWriterState*,
    std::function<void({{proto_field_cpp_type}}*)> setter) {
    {{proto_field_cpp_type}}* proto_{{attribute_name}} = new {{proto_field_cpp_type}}();
    {% for attribute_component in attribute_components %}
        proto_{{attribute_name}}->set_{{attribute_component.protobuf_field}}(value.{{attribute_component.attribute_name}});
    {% endfor %}
    setter(proto_{{attribute_name}});
}
