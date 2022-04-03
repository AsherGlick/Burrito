#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include <string>
#include <vector>
#include "filter.hpp"
#include <iostream>
#include <map>

using namespace std;

bool Filter::setup_variable(void (*function)(void* filter_object), void* object, vector<string> names) {
	for (auto name: names) {
		if (this->setter_lookup.count(name)) {
			throw;
		}
		this->setter_lookup[name] = function;
	}
	return false;
}


////////////////////////////////////////////////////////////////////////////////
// Filter::parse
//
// Runs through all of the items that are defined with FILTER_ITEM() to be used
// as valid elemetns of this filter, and parses them out into their individual
// boolean values. This function should be called by the respective subclass
// parse functions to handle the extraction automatically. 
////////////////////////////////////////////////////////////////////////////////
void Filter::parse(rapidxml::xml_attribute<>* input, vector<string> *errors) {
	vector<string> items = split(string(input->value()), ",");

	for (string item : items) {
		auto iterator = this->setter_lookup.find(item);

		if (iterator == this->setter_lookup.end()) {
			errors->push_back("Unknown " + this->classname() + " option " + item);
			continue;
		}

		iterator->second(this);
	}
}
