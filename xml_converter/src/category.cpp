#include "category.hpp"

using namespace std;

string Cateogry::classname() {
    return "MarkerCategory";
}

void Cateogry::init_from_xml(rapidxml::xml_node<>* node, vector<string> *errors) {
    for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        bool is_icon_value = this->default_icon.init_xml_attribute(attribute, errors);
        bool is_trail_value = this->default_trail.init_xml_attribute(attribute, errors);

        if (init_xml_attribute(attribute, errors)) {}
        else if (is_icon_value || is_trail_value) {}
        else {
            errors->push_back("Unknown " + this->classname() + " attribute " + string(attribute->name()));
        }
    }
}

