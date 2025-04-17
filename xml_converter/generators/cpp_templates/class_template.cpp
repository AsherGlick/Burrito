#include "{{cpp_class_header}}_gen.hpp"

#include <algorithm>
{% for absolute_include in cpp_includes.sorted_cpp_absolute_includes() %}
    #include <{{absolute_include}}>
{% endfor %}

{% for relative_include in cpp_includes.sorted_cpp_relative_includes() %}
    #include "{{relative_include}}"
{% endfor %}

{% for forward_declaration in cpp_includes.sorted_cpp_forward_declarations() %}
    class {{forward_declaration}};
{% endfor %}
using namespace std;

string {{cpp_class}}::classname() {
    return "{{xml_class_name}}";
}
{% if cpp_class == "Category" %}
    void {{cpp_class}}::init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*>* errors, XMLReaderState* state) {
        for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
            bool handled = false;

            if (init_xml_attribute(attribute, errors, state)) {
                handled = true;
            }
            // Attempt to parse the attributes and throw away the results. We
            // perform this extra work here to identify if the attribute is
            // part of the particular marker, and to gather any errors there
            // might be with that attribute so they can be correctly attributed
            // to this node instead of the children that inherit the field.
            if (Icon().init_xml_attribute(attribute, errors, state)) {
                this->icon_attributes.push_back(attribute);
                handled = true;
            }
            if (Trail().init_xml_attribute(attribute, errors, state)) {
                this->trail_attributes.push_back(attribute);
                handled = true;
            }

            if (!handled) {
                errors->push_back(new XMLAttributeNameError("Unknown " + this->classname() + " attribute ", attribute));
            }
        }
    }
{% endif %}

bool {{cpp_class}}::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*>* errors, XMLReaderState* state) {
    string attributename;
    attributename = get_attribute_name(attribute);
    {% for n, attribute_variable in enumerate(attribute_variables) %}
        {% for i, value in enumerate(attribute_variable.xml_info.xml_fields) %}
            {{ "if" if i == n == 0 else "else if" }} (attributename == "{{value}}") {
                {{attribute_variable.xml_info.deserialize_xml_function}}(attribute, errors, state, &(this->{{attribute_variable.attribute_name}}), &(this->{{attribute_variable.attribute_flag_name}}){% for side_effect in attribute_variable.xml_info.deserialize_xml_side_effects %}, &(this->{{side_effect}}){% endfor %});
            }
        {% endfor %}
    {% endfor %}
    else {
        return false;
    }
    return true;
}
{% if attributes_of_type_marker_category %}

    bool {{cpp_class}}::validate_attributes_of_type_marker_category() {
        {% for attribute in attributes_of_type_marker_category %}
        // TODO: validate "{{attribute}}"
        {% endfor %}
        return true;
    }
{% endif %}

rapidxml::xml_node<char>* {{cpp_class}}::as_xml(XMLWriterState* state) const {
    rapidxml::xml_node<char>* xml_node = state->doc->allocate_node(rapidxml::node_element, "{{xml_class_name}}");

    {% for attribute_variable in attribute_variables %}
        {% if attribute_variable.xml_info.write_to_xml == true %}
            if (this->{{attribute_variable.attribute_flag_name}}) {
                std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("{{attribute_variable.xml_info.default_xml_field}}", state->doc->allocate_string(val.c_str()))); };
                {{attribute_variable.xml_info.serialize_xml_function}}(state, &this->{{attribute_variable.attribute_name}}{% for side_effect in attribute_variable.xml_info.serialize_xml_side_effects %}, &(this->{{side_effect}}){% endfor %}, setter);
            }
        {% endif %}
    {% endfor %}
    {% if cpp_class == "Category" %}

        for (const auto& [key, val] : this->children) {
            xml_node->append_node(val.as_xml(state));
        }
    {% endif %}
    return xml_node;
}

// The following attributes are not yet supported in Burrito
// and are not written to or read from the protobuf file:
{% for attribute_variable in attribute_variables %}
    {% if attribute_variable.exclude_from_protobuf == true %}
        //  {{attribute_variable.attribute_name}}
    {% endif %}
{% endfor %}

guildpoint::{{cpp_class}} {{cpp_class}}::as_protobuf(ProtoWriterState* state) const {
    guildpoint::{{cpp_class}} proto_{{cpp_class_header}};
    {% for attribute_variable in attribute_variables %}
        {% if attribute_variable.is_component == false and attribute_variable.proto_info != None %}
            if (this->{{attribute_variable.attribute_flag_name}}) {
                {% if not attribute_variable.proto_info.is_proto_field_scalar %}
                    std::function<void({{attribute_variable.proto_info.protobuf_cpp_type}}*)> setter = [&proto_{{cpp_class_header}}]({{attribute_variable.proto_info.protobuf_cpp_type}}* val) { proto_{{cpp_class_header}}.{{attribute_variable.proto_info.mutable_proto_drilldown_calls}}set_allocated_{{attribute_variable.proto_info.protobuf_field}}(val); };
                {% else %}
                    std::function<void({{attribute_variable.proto_info.protobuf_cpp_type}})> setter = [&proto_{{cpp_class_header}}]({{attribute_variable.proto_info.protobuf_cpp_type}} val) { proto_{{cpp_class_header}}.{{attribute_variable.proto_info.mutable_proto_drilldown_calls}}set_{{attribute_variable.proto_info.protobuf_field}}(val); };
                {% endif %}
                {{attribute_variable.proto_info.serialize_proto_function}}(this->{{attribute_variable.attribute_name}}, state, setter{% for side_effect in attribute_variable.proto_info.serialize_proto_side_effects %}, &(this->{{side_effect}}){% endfor %});
            }
        {% endif %}
    {% endfor %}
    return proto_{{cpp_class_header}};
}

void {{cpp_class}}::parse_protobuf(guildpoint::{{cpp_class}} proto_{{cpp_class_header}}, ProtoReaderState* state) {
    {% for attribute_variable in attribute_variables %}
        {% if attribute_variable.is_component == false and attribute_variable.proto_info != None %}
            {% if not attribute_variable.proto_info.is_proto_field_scalar %}
                if (proto_{{cpp_class_header}}{{attribute_variable.proto_info.proto_drilldown_calls}}.has_{{attribute_variable.proto_info.protobuf_field}}()) {
            {% elif attribute_variable.proto_info.protobuf_cpp_type == "std::string" %}
                if (proto_{{cpp_class_header}}{{attribute_variable.proto_info.proto_drilldown_calls}}.{{attribute_variable.proto_info.protobuf_field}}() != "") {
            {% else %}
                if (proto_{{cpp_class_header}}{{attribute_variable.proto_info.proto_drilldown_calls}}.{{attribute_variable.proto_info.protobuf_field}}() != 0) {
            {% endif %}
                {{attribute_variable.proto_info.deserialize_proto_function}}(proto_{{cpp_class_header}}{{attribute_variable.proto_info.proto_drilldown_calls}}.{{attribute_variable.proto_info.protobuf_field}}(), state, &(this->{{attribute_variable.attribute_name}}), &(this->{{attribute_variable.attribute_flag_name}}){% for side_effect in attribute_variable.proto_info.deserialize_proto_side_effects %}, &(this->{{side_effect}}){% endfor %});
            }
        {% endif %}
    {% endfor %}
}
