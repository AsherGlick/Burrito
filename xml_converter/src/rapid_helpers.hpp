#pragma once

#include <string>

#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

string find_attribute_value(rapidxml::xml_node<>* node, string attribute_name);
rapidxml::xml_attribute<>* find_attribute(rapidxml::xml_node<>* node, string attribute_name);

string get_attribute_name(rapidxml::xml_attribute<>* attribute);
string get_attribute_value(rapidxml::xml_attribute<>* attribute);

string get_node_name(rapidxml::xml_node<>* node);


class XMLError {
 protected:
	string message;
 public:
	virtual void print_error(string source) = 0;
};


// rapidxml::xml_node<>* root_node
// rapidxml::xml_attribute<>* input

class XMLStringError: public XMLError {
 public:
	XMLStringError(string message);
	void print_error(string source);
};

class XMLAttributeNameError: public XMLError {
 protected:
	rapidxml::xml_attribute<>* attribute;
 public:
	XMLAttributeNameError(string message, rapidxml::xml_attribute<>* attribute);
	void print_error(string source);
};

class XMLAttributeValueError: public XMLError {
 protected:
	rapidxml::xml_attribute<>* attribute;
 public:
	XMLAttributeValueError(string message, rapidxml::xml_attribute<>* attribute);
	void print_error(string source);
};

class XMLNodeNameError: public XMLError {
 protected:
	rapidxml::xml_node<>* node;
 public:
	XMLNodeNameError(string message, rapidxml::xml_node<>* node);
	void print_error(string source);
};
