#include "category.hpp"
#include "attribute/string.hpp"
#include "attribute/bool.hpp"

using namespace std;


string Cateogry::classname() {
	return "MarkerCategory";
}

void Cateogry::init_from_xml(rapidxml::xml_node<>* node, vector<string> *errors) {
	for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()){
		if (init_xml_attribute(attribute, errors)) {}
		else {
			errors->push_back("Unknown " + this->classname() + " attribute " + string(attribute->name()));
		}
	} 
}

bool Cateogry::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<string> *errors) {
	return false;
}