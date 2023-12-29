#pragma once

{% for absolute_include in cpp_includes.sorted_hpp_absolute_includes() %}
    #include <{{absolute_include}}>
{% endfor %}

{% for relative_include in cpp_includes.sorted_hpp_relative_includes() %}
    #include "{{relative_include}}"
{% endfor %}

{% for forward_declaration in cpp_includes.sorted_hpp_forward_declarations() %}
    class {{forward_declaration}};
{% endfor %}

class {{cpp_class}} : public Parseable {
 public:
    {% for attribute_variable in attribute_variables %}
        {% if attribute_variable.is_component == false %}
            {{attribute_variable.cpp_type}} {{attribute_variable.attribute_name}};
        {% endif %}
    {% endfor %}
    {% for attribute_variable in attribute_variables %}
        {% if attribute_variable.is_component == false %}
            bool {{attribute_variable.attribute_flag_name}} = false;
        {% endif %}
    {% endfor %}
    {% if cpp_class == "Category" %}
        std::map<std::string, Category> children;
        Icon default_icon;
        Trail default_trail;

        void init_from_xml(rapidxml::xml_node<>* node, std::vector<XMLError*>* errors, XMLReaderState* state);
    {% endif %}
    virtual std::vector<std::string> as_xml(XMLWriterState* state) const;
    virtual std::string classname();
    bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, std::vector<XMLError*>* errors, XMLReaderState* state);
    waypoint::{{cpp_class}} as_protobuf(ProtoWriterState* state) const;
    void parse_protobuf(waypoint::{{cpp_class}} proto_{{cpp_class_header}}, ProtoReaderState* state);
    {% if attributes_of_type_marker_category %}
        bool validate_attributes_of_type_marker_category();
    {% endif %}
};
