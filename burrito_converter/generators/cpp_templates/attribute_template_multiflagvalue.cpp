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

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads a {{class_name}} from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::{{namespace}}::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    {{class_name}}* value,
    bool* is_set
) {
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
    vector<string> flag_values;
    {% for n, attribute_component in enumerate(attribute_components) %}
        if (value->{{attribute_component.attribute_name}} == true) {
            flag_values.push_back("{{attribute_component.xml_fields[0]}}");
        }
    {% endfor %}
    string output = join(flag_values, ",");
    setter(output);
}
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
        {% for n, attribute_component in enumerate(attribute_components) %}
            {{attribute_name}}.{{attribute_component.attribute_name}} = input.{{attribute_component.attribute_name}}();
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
        bool should_write = false;
        {% for n, attribute_component in enumerate(attribute_components) %}
            proto_{{attribute_name}}->set_{{attribute_component.attribute_name}}(value.{{attribute_component.attribute_name}});
            should_write |= value.{{attribute_component.attribute_name}};
        {% endfor %}
        if (should_write) {
            setter(proto_{{attribute_name}});
        }
    }
{% endif %}
