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
    std::vector<XMLError*>* errors,
    {{class_name}}* value,
    bool* is_set) {
    {{class_name}} {{attribute_name}};
    vector<string> compound_values;
    string attributename;
    {% for attribute_variable in attribute_variables: %}
        {{attribute_name}}.{{attribute_variable.attribute_name}} = 0;
    {% endfor %}
    attributename = get_attribute_name(input);
    compound_values = split(get_attribute_value(input), ",");
    if (compound_values.size() == {{ attribute_variables|length }}) {
        {% for n, attribute_variable in enumerate(attribute_variables) %}
            {{attribute_name}}.{{attribute_variable.attribute_name}} = std::stof(compound_values[{{n}}]);
        {% endfor %}
    }
    *value = {{attribute_name}};
    *is_set = true;
}
{% if xml_bundled_components != [] %}
    string {{attribute_name}}_to_xml_attribute(const std::string& attribute_name, const {{class_name}}* value) {
        string output;
        {% for n, attribute_variable in enumerate(attribute_variables) %}
            {% if attribute_variable.attribute_name in xml_bundled_components %}
                {% if n == 0: %}
                    output = to_string(value->{{attribute_variable.attribute_name}});
                {% else %}
                    output = output + "," + to_string(value->{{attribute_variable.attribute_name}});
                {% endif %}
            {% endif %}
        {% endfor %}
        return " " + attribute_name + "=\"" + output + "\"";
    }
{% endif %}

{{class_name}} from_proto_{{attribute_name}}({{proto_field_cpp_type}} proto_{{attribute_name}}) {
    {{class_name}} {{attribute_name}};
    {% for attribute_variable in attribute_variables: %}
        {{attribute_name}}.{{attribute_variable.attribute_name}} = proto_{{attribute_name}}.{{attribute_variable.protobuf_field}}();
    {% endfor %}
    return {{attribute_name}};
}

void {{attribute_name}}_to_proto({{class_name}} value, std::function<void({{proto_field_cpp_type}}*)> setter) {
    {{proto_field_cpp_type}}* proto_{{attribute_name}} = new {{proto_field_cpp_type}}();
    {% for attribute_variable in attribute_variables %}
        proto_{{attribute_name}}->set_{{attribute_variable.protobuf_field}}(value.{{attribute_variable.attribute_name}});
    {% endfor %}
    setter(proto_{{attribute_name}});
}