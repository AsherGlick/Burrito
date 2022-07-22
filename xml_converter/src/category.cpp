#include "category.hpp"

using namespace std;

string Category::classname() {
    return "MarkerCategory";
}

void Category::init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*> *errors) {
    for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {

        // Attempt to parse all the attributes of the category into an Icon and
        // Trail to use as default values for all their children.
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

