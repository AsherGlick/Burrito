#pragma once

#include <string>

#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

string find_attribute_value(rapidxml::xml_node<>* node, string attribute_name);
rapidxml::xml_attribute<>* find_attribute(rapidxml::xml_node<>* node, string attribute_name);

string get_attribute_name(rapidxml::xml_attribute<>* attribute);
string get_attribute_value(rapidxml::xml_attribute<>* attribute);

string get_node_name(rapidxml::xml_node<>* node);


////////////////////////////////////////////////////////////////////////////////
// XMLError
//
// A base class for all the XML errors. It itself should not be used as an
// error, but all of it's subclasses can be.
////////////////////////////////////////////////////////////////////////////////
class XMLError {
 protected:
	string error_message;
 public:
	void print_error();
};

class XMLAttributeNameError: public XMLError {
 public:
	XMLAttributeNameError(string message, rapidxml::xml_attribute<>* attribute);
};

class XMLAttributeValueError: public XMLError {
 public:
	XMLAttributeValueError(string message, rapidxml::xml_attribute<>* attribute);
};

class XMLNodeNameError: public XMLError {
 public:
	XMLNodeNameError(string message, rapidxml::xml_node<>* node);
};
