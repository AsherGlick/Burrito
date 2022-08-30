#include "category_gen.hpp"
#include <typeinfo>
#include <string>

using namespace std;

string Category::classname() {
    return "MarkerCategory";
}
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
bool Category::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors) {
    string attributename; 
    attributename = normalize(get_attribute_name(attribute)); 
    if (attributename == "tipdescription") {
        this->tooltip_name = parse_string(attribute, errors);
        this->tooltip_name_is_true = true;
    }
    else if (attributename == "name") {
        this->name = parse_string(attribute, errors);
        this->name_is_true = true;
    }
    else if (attributename == "displayname") {
        this->display_name = parse_string(attribute, errors);
        this->display_name_is_true = true;
    }
    else if (attributename == "isseparator") {
        this->is_separator = parse_bool(attribute, errors);
        this->is_separator_is_true = true;
    }
    else if (attributename == "defaulttoggle") {
        this->default_visibility = parse_bool(attribute, errors);
        this->default_visibility_is_true = true;
    }
    else {
        return false;
    }
    return true;
}

vector<string> Category::as_xml() const {
    vector<string> xml_node_contents;
    xml_node_contents.push_back("<MarkerCategory ");
    if (this->tooltip_name_is_true) {
        xml_node_contents.push_back(" TipDescription=\"" + stringify_string(this->tooltip_name) + "\"");
    }
    if (this->name_is_true) {
        xml_node_contents.push_back(" Name=\"" + stringify_string(this->name) + "\"");
    }
    if (this->display_name_is_true) {
        xml_node_contents.push_back(" DisplayName=\"" + stringify_string(this->display_name) + "\"");
    }
    if (this->is_separator_is_true) {
        xml_node_contents.push_back(" IsSeparator=\"" + stringify_bool(this->is_separator) + "\"");
    }
    if (this->default_visibility_is_true) {
        xml_node_contents.push_back(" DefaultToggle=\"" + stringify_bool(this->default_visibility) + "\"");
    }    
    xml_node_contents.push_back(">\n");

    for (const auto& [key, val] : this->children){
        string text; 
        for (const auto& s: val.as_xml()) { text += s; };

        xml_node_contents.push_back("\t" + text);
    }

    xml_node_contents.push_back("</MarkerCategory>\n");
    return xml_node_contents;
}