#include "category.hpp"

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
    if (attributename == "defaulttoggle") {
        this->default_visibility = parse_bool(attribute, errors);
    }
    else if (attributename == "displayname") {
        this->display_name = parse_string(attribute, errors);
    }
    else if (attributename == "isseparator") {
        this->is_separator = parse_bool(attribute, errors);
    }
    else if (attributename == "name") {
        this->name = parse_string(attribute, errors);
    }
    else if (attributename == "tipdescription") {
        this->tooltip_name = parse_string(attribute, errors);
    }
    else {
        return false;
    }
    return true;
}