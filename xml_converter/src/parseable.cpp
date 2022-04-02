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
	auto iterator = this->variable_list.find(normalize_type_name(attribute->name()));

	if (iterator == this->variable_list.end()) {
		return false;;
	}

	RemoteCall function_call = iterator->second;
	function_call.function(function_call.object, attribute, errors);
	return true;
}


bool Parseable::setup_variable(
		void (*function)(void*, rapidxml::xml_attribute<>*, vector<string>*),
		void* object,
		vector<string> names
) {

	for (auto name : names) {
		if (this->variable_list.count(name)) {
			throw;
		}
		this->variable_list[normalize_type_name(name)] = {function, object};
	}

	return false;
}
