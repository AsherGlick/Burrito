#include "rapidxml-1.13/rapidxml.hpp"

#include "parseable.hpp"

#include "string_helper.hpp"

#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <string.h>
#include <functional>
#include <cstdio>
using namespace std;

Parseable::Parseable() {}

string Parseable::classname() {
	return "Parseable";
}


void Parseable::init_from_xml(rapidxml::xml_node<>* node, vector<string> *errors) {
	
	for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()){
		if (init_xml_attribute(attribute, errors)) {}
		else {
			errors->push_back("Unknown " + this->classname() + " attribute " + string(attribute->name()));
		}
	}
}

bool Parseable::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<string> *errors) {
	for (auto parser_function : this->variable_list) {
		if (nomralized_matches_any(attribute->name(), parser_function.token_names)) {
			// if (string(attribute->name()) != attr_name) {
				/*errors->push_back("Found a similar but incorrect attribute \"" + string(attribute->name()) + "\" treating it as \"" + attr_name + "\".");*/ \
			// }
			parser_function.function(parser_function.object, attribute, errors);
			return true;
		}
	}

	return false;
}


bool Parseable::setup_variable(
		void (*function)(void*, rapidxml::xml_attribute<>*, vector<string>*),
		void* object,
		vector<string> names
) {
	this->variable_list.push_back({
		function,
		object,
		names
	});
	return false;
}
