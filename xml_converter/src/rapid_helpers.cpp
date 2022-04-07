#include "rapid_helpers.hpp"
#include <iostream>
using namespace std;

string find_attribute_value(rapidxml::xml_node<>* node, string attribute_name) {
    auto attribute = node->first_attribute(attribute_name.data(), attribute_name.size(), false);
    
    return string(attribute->value(), attribute->value_size());
}

rapidxml::xml_attribute<>* find_attribute(rapidxml::xml_node<>* node, string attribute_name) {
    return node->first_attribute(attribute_name.data(), attribute_name.size(), false);
}

string get_attribute_name(rapidxml::xml_attribute<>* attribute) {
    return string(attribute->name(), attribute->name_size());
}

string get_attribute_value(rapidxml::xml_attribute<>* attribute) {
    return string(attribute->value(), attribute->value_size());
}

string get_node_name(rapidxml::xml_node<>* node) {
    return string(node->name(), node->name_size());
}





XMLStringError::XMLStringError(string message) {
    this->message = message;
}
    
void XMLStringError::print_error(string source) {
    cout << this->message << endl;
}


XMLAttributeNameError::XMLAttributeNameError(string message, rapidxml::xml_attribute<>* attribute) {
    this->message = message;
    this->attribute = attribute;
}
void XMLAttributeNameError::print_error(string source) {
    cout << "Attribute Name Error: " << this->message << get_attribute_name(this->attribute) << endl;

}

XMLAttributeValueError::XMLAttributeValueError(string message, rapidxml::xml_attribute<>* attribute) {
    this->message = message;
    this->attribute = attribute;
}
void XMLAttributeValueError::print_error(string source) {
    cout << "Attribute Value Error: " << this->message << get_attribute_value(this->attribute) << endl;
}


XMLNodeNameError::XMLNodeNameError(string message, rapidxml::xml_node<>* node) {
    this->message = message;
    this->node = node;
}
void XMLNodeNameError::print_error(string source){
    cout << "XMLNodeNameError: " << this->message << get_node_name(this->node) << endl;
}


