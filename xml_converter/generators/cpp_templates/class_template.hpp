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
    {% for attribute_variable in attribute_variables: %}
    {{attribute_variable.cpp_type}} {{attribute_variable.attribute_name}};
    {% endfor %}
    {% for attribute_variable in attribute_variables: %}
    bool {{attribute_variable.attribute_name}}_is_set = false;
    {% endfor %}
    {% if cpp_class == "Icon": %}
    bool set_trigger = false;
    {% endif %}
    {% if cpp_class == "Category": %}
    std::map<std::string, Category> children;
    Icon default_icon;
    Trail default_trail;


    void init_from_xml(rapidxml::xml_node<>* node, std::vector<XMLError*>* errors);
    {% endif %}
    virtual std::vector<std::string> as_xml() const;
    virtual std::string classname();
    bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, std::vector<XMLError*>* errors);
    virtual std::string as_protobuf() const; 
    {% if attributes_of_type_marker_category %}
    bool validate_attributes_of_type_marker_category();
    {% endif %}
};
