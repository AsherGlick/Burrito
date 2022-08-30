#include "{{cpp_class_header}}_gen.hpp"
#include <typeinfo>
#include <string>

using namespace std;

string {{cpp_class}}::classname() {
    return "{{xml_class_name}}";
}
{%- if cpp_class == "Category": %}
void Category::init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*> *errors) {
    for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        bool is_icon_value = false;
        bool is_trail_value = false;

        for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
            is_icon_value |= this->default_icon.init_xml_attribute(attribute, errors);
            is_trail_value |= this->default_trail.init_xml_attribute(attribute, errors);
        }

        if (init_xml_attribute(attribute, errors)) {}
        else if (is_icon_value || is_trail_value) {}
        else {
            errors->push_back(new XMLAttributeNameError("Unknown " + this->classname() + " attribute ", attribute));    
        }
    }
}

{%- endif %}
bool {{cpp_class}}::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors) {
    string attributename; 
    attributename = normalize(get_attribute_name(attribute));



{%-for n, attribute_variable in enumerate(attribute_variables)%}    
    {%-for i, value in enumerate(attribute_variable.xml_fields)%}
        {%-if i == 0 and n == 0:%} 
    if (attributename == "{{value}}") {
        this->{{attribute_variable.attribute_name}} = parse_{{attribute_variable.class_name}}(attribute, errors);
        this->{{attribute_variable.attribute_name}}_is_true = true;
    }
        {%-elif (attribute_variable.attribute_type == "CompoundValue" and attribute_variable.class_name != attribute_variable.attribute_name) %}
    else if (attributename == "{{value}}") {
        this->{{attribute_variable.attribute_name}} = parse_float(attribute, errors);
        this->{{attribute_variable.attribute_name}}_is_true = true;
    }
        {%- else: %}
    else if (attributename == "{{value}}") {
        this->{{attribute_variable.attribute_name}} = parse_{{attribute_variable.class_name}}(attribute, errors);
        this->{{attribute_variable.attribute_name}}_is_true = true;
    }
        {%- endif %}
    {%- endfor %}
{%- endfor %}
    else {
        return false;
    }
    return true;
}

{%-if attributes_of_type_marker_category %}
bool {{cpp_class}}::validate_attributes_of_type_marker_category() {
    {%-for attribute in attributes_of_type_marker_category%}
    // TODO: validate "{{attribute}}"
    {%- endfor %}
    return true;
}
{%- endif %}

vector<string> {{cpp_class}}::as_xml() const {
    vector<string> xml_node_contents;
    xml_node_contents.push_back("<{{xml_class_name}} ");
{%-for attribute_variable in attribute_variables%}
    {%- if (attribute_variable.attribute_type == "CompoundValue")%}
        {%-if (attribute_variable.xml_export == "Children" and attribute_variable.is_child == true)%}
    if (this->{{attribute_variable.attribute_name}}_is_true) {    
        xml_node_contents.push_back(" {{attribute_variable.default_xml_fields[0]}}=\"" + to_string(this->{{attribute_variable.attribute_name}}) + "\"");
    } 
        {%-elif (attribute_variable.xml_export == "Parent" and attribute_variable.is_child == false)%}
    if (this->{{attribute_variable.attribute_name}}_is_true) {   
        xml_node_contents.push_back(" {{attribute_variable.default_xml_fields[0]}}=\"" + stringify_{{attribute_variable.class_name}}(this->{{attribute_variable.attribute_name}}) + "\"");
    }
       {%-elif (attribute_variable.xml_export == "Parent and Children")%}
            {%-for value in attribute_variable.xml_fields%} 
    if (this->{{attribute_variable.attribute_name}}_is_true) {               
        xml_node_contents.push_back(" {{value}}=\"" + stringify_{{attribute_variable.class_name}}(this->{{attribute_variable.attribute_name}}) + "\"");
            {%- endfor %} 
    }
        {%- endif %} 
    {%- else: %}
    if (this->{{attribute_variable.attribute_name}}_is_true) {
        xml_node_contents.push_back(" {{attribute_variable.default_xml_fields[0]}}=\"" + stringify_{{attribute_variable.class_name}}(this->{{attribute_variable.attribute_name}}) + "\"");
    }
    {%- endif %}   
    
{%- endfor %}
{%- if cpp_class == "Category": %}    
    xml_node_contents.push_back(">\n");

    for (const auto& [key, val] : this->children){
        string text; 
        for (const auto& s: val.as_xml()) { text += s; };

        xml_node_contents.push_back("\t" + text);
    }

    xml_node_contents.push_back("</MarkerCategory>\n");    
{%- else: %}
    xml_node_contents.push_back("/>");
{%- endif %}
    return xml_node_contents;
}
