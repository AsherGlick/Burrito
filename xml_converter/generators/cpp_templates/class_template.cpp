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
{% if cpp_class == "Category": %}
    void {{cpp_class}}::init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*>* errors, string base_dir) {
        for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
            bool is_icon_value = this->default_icon.init_xml_attribute(attribute, errors);
            bool is_trail_value = this->default_trail.init_xml_attribute(attribute, errors);

            if (init_xml_attribute(attribute, errors, base_dir)) {
            }
            else if (is_icon_value || is_trail_value) {
            }
            else {
                errors->push_back(new XMLAttributeNameError("Unknown " + this->classname() + " attribute ", attribute));
            }
        }
    }
{% endif %}

bool {{cpp_class}}::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*>* errors, string base_dir) {
    string attributename;
    attributename = normalize(get_attribute_name(attribute));
    {% for n, attribute_variable in enumerate(attribute_variables) %}
        {% for i, value in enumerate(attribute_variable.xml_fields) %}
            {{ "if" if i == n == 0 else "else if" }} (attributename == "{{value}}") {
                this->{{attribute_variable.attribute_name}} = parse_{{attribute_variable.class_name}}({{", ".join(attribute_variable.args)}});
                this->{{attribute_variable.attribute_flag_name}} = true;
                {% for side_effect in attribute_variable.side_effects %}
                    this->{{side_effect}} = this->{{attribute_variable.class_name}}.side_effect_{{side_effect}};
                    this->{{side_effect}}_is_set = true;
                {% endfor %}
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

vector<string> {{cpp_class}}::as_xml() const {
    vector<string> xml_node_contents;
    xml_node_contents.push_back("<{{xml_class_name}} ");
    {% for attribute_variable in attribute_variables %}
        {% if attribute_variable.write_to_xml == true %}
            if (this->{{attribute_variable.attribute_flag_name}}) {
                xml_node_contents.push_back(" {{attribute_variable.default_xml_field}}=\"" + stringify_{{attribute_variable.class_name}}(this->{{attribute_variable.attribute_name}}) + "\"");
            }
        {% endif %}
    {% endfor %}
    {% if cpp_class == "Category": %}
        xml_node_contents.push_back(">\n");

        for (const auto& [key, val] : this->children) {
            string text;
            for (const auto& s : val.as_xml()) {
                text += s;
            }

            xml_node_contents.push_back("\t" + text);
        }

        xml_node_contents.push_back("</MarkerCategory>\n");
    {% else %}
        xml_node_contents.push_back("/>");
    {% endif %}
    return xml_node_contents;
}

waypoint::{{cpp_class}} {{cpp_class}}::as_protobuf() const {
    waypoint::{{cpp_class}} proto_{{cpp_class_header}};
    {% if cpp_class == "Icon": %}
        waypoint::Trigger* trigger = nullptr;
    {% endif %}
    {% for attribute_variable in attribute_variables %}
        {% if attribute_variable.is_component == false %}
            {% if (attribute_variable.proto_drilldown_calls != "")%}{# TODO: This is a hack to preserve functionality when removing is_trigger #}
                {% if (attribute_variable.attribute_type == "Custom")%}
                    if (this->{{attribute_variable.attribute_flag_name}}) {
                        if (trigger == nullptr) {
                            trigger = new waypoint::Trigger();
                        }
                        trigger->set_allocated_{{attribute_variable.protobuf_field}}(to_proto_{{attribute_variable.class_name}}(this->{{attribute_variable.attribute_name}}));
                    }
                {% elif (attribute_variable.attribute_type == "Enum")%}
                    if (this->{{attribute_variable.attribute_flag_name}}) {
                        if (trigger == nullptr) {
                            trigger = new waypoint::Trigger();
                        }
                        trigger->set_{{attribute_variable.protobuf_field}}(to_proto_{{attribute_variable.class_name}}(this->{{attribute_variable.attribute_name}}));
                    }
                {% else: %}
                    if (this->{{attribute_variable.attribute_flag_name}}) {
                        if (trigger == nullptr) {
                            trigger = new waypoint::Trigger();
                        }
                        trigger->set_{{attribute_variable.protobuf_field}}(this->{{attribute_variable.attribute_name}});
                    }
                {% endif %}
            {% else: %}
                {% if (attribute_variable.attribute_type == "Enum")%}
                    if (this->{{attribute_variable.attribute_flag_name}}) {
                        proto_{{cpp_class_header}}.set_{{attribute_variable.protobuf_field}}(to_proto_{{attribute_variable.class_name}}(this->{{attribute_variable.attribute_name}}));
                    }
                {% elif (attribute_variable.attribute_type in ["MultiflagValue", "CompoundValue", "Custom", "CompoundCustomClass"])%}
                    if (this->{{attribute_variable.attribute_flag_name}}) {
                        proto_{{cpp_class_header}}.set_allocated_{{attribute_variable.protobuf_field}}(to_proto_{{attribute_variable.class_name}}(this->{{attribute_variable.attribute_name}}));
                    }
                {% else: %}
                    if (this->{{attribute_variable.attribute_flag_name}}) {
                        proto_{{cpp_class_header}}.set_{{attribute_variable.protobuf_field}}(this->{{attribute_variable.attribute_name}});
                    }
                {% endif %}
            {% endif %}
        {% endif %}
    {% endfor %}
    {% if cpp_class == "Icon": %}
        if (trigger != nullptr) {
            proto_{{cpp_class_header}}.set_allocated_trigger(trigger);
        }
    {% endif %}
    return proto_{{cpp_class_header}};
}

void {{cpp_class}}::parse_protobuf(waypoint::{{cpp_class}} proto_{{cpp_class_header}}) {
    {% for attribute_variable in attribute_variables %}
        {% if attribute_variable.is_component == false %}
            {% if (attribute_variable.attribute_type in ["MultiflagValue", "CompoundValue", "Custom", "CompoundCustomClass"]) %}
                if (proto_{{cpp_class_header}}{{attribute_variable.proto_drilldown_calls}}.has_{{attribute_variable.protobuf_field}}()) {
                    this->{{attribute_variable.attribute_name}} = from_proto_{{attribute_variable.class_name}}(proto_{{cpp_class_header}}{{attribute_variable.proto_drilldown_calls}}.{{attribute_variable.protobuf_field}}());
                    this->{{attribute_variable.attribute_flag_name}} = true;
                }
            {% elif (attribute_variable.class_name == "string") %}{# TODO: why is this .class_name when the others are .attribute_name #}
                if (proto_{{cpp_class_header}}{{attribute_variable.proto_drilldown_calls}}.{{attribute_variable.protobuf_field}}() != "") {
                    this->{{attribute_variable.attribute_name}} = proto_{{cpp_class_header}}{{attribute_variable.proto_drilldown_calls}}.{{attribute_variable.protobuf_field}}();
                    this->{{attribute_variable.attribute_flag_name}} = true;
                }
            {% elif (attribute_variable.attribute_type ==  "Enum") %}
                if (proto_{{cpp_class_header}}{{attribute_variable.proto_drilldown_calls}}.{{attribute_variable.protobuf_field}}() != 0) {
                    this->{{attribute_variable.attribute_name}} = from_proto_{{attribute_variable.class_name}}(proto_{{cpp_class_header}}{{attribute_variable.proto_drilldown_calls}}.{{attribute_variable.protobuf_field}}());
                    this->{{attribute_variable.attribute_flag_name}} = true;
                }
            {% else %}
                if (proto_{{cpp_class_header}}{{attribute_variable.proto_drilldown_calls}}.{{attribute_variable.protobuf_field}}() != 0) {
                    this->{{attribute_variable.attribute_name}} = proto_{{cpp_class_header}}{{attribute_variable.proto_drilldown_calls}}.{{attribute_variable.protobuf_field}}();
                    this->{{attribute_variable.attribute_flag_name}} = true;
                }
            {% endif %}
        {% endif %}
    {% endfor %}
}
