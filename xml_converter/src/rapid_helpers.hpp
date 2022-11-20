#pragma once

#include <string>

#include "rapidxml-1.13/rapidxml.hpp"

std::string find_attribute_value(rapidxml::xml_node<>* node, std::string attribute_name);
rapidxml::xml_attribute<>* find_attribute(rapidxml::xml_node<>* node, std::string attribute_name);

std::string get_attribute_name(rapidxml::xml_attribute<>* attribute);
std::string get_attribute_value(rapidxml::xml_attribute<>* attribute);

std::string get_node_name(rapidxml::xml_node<>* node);

////////////////////////////////////////////////////////////////////////////////
// XMLError
//
// A base class for all the XML errors. It itself should not be used as an
// error, but all of it's subclasses can be.
////////////////////////////////////////////////////////////////////////////////
class XMLError {
 protected:
    std::string error_message;

 public:
    void print_error();
};

class XMLAttributeNameError : public XMLError {
 public:
    XMLAttributeNameError(std::string message, rapidxml::xml_attribute<>* attribute);
};

class XMLAttributeValueError : public XMLError {
 public:
    XMLAttributeValueError(std::string message, rapidxml::xml_attribute<>* attribute);
};

class XMLNodeNameError : public XMLError {
 public:
    XMLNodeNameError(std::string message, rapidxml::xml_node<>* node);
};
