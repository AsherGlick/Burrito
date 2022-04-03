#include "filter.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

uint64_t Filter::_counter = 0;
map<string, uint64_t> Filter::original;
map<string, map<string, void (*)(void* filter_object)>> Filter::lookup;

bool Filter::setup_variable(void (*function)(void* filter_object), void* object, vector<string> names) {
    const char* type_id = typeid(*this).name();
    auto iterator = original.find(type_id);

    // If this instance is not the first instance of the subclass this
    // execution then exit early to avoid rebuilding the lookup map.
    if (iterator != original.end() && iterator->second != this->_id) {
        return false;
    }

    // Set this instance's id as the id of the first instance of this subclass
    // to prevent any other instances from rebuilding the lookup map.
    original[type_id] = this->_id;

    // Grab a pointer to the lookup data for this subclass so we can edit it.
    map<string, void (*)(void* filter_object)>* variable_list = &lookup[type_id];

    // Insert all of the names for this field, error on duplicates.
    for (auto name : names) {
        if (variable_list->count(name)) {
            throw;
        }
        (*variable_list)[name] = function;
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

    const char* type_id = typeid(*this).name();
    auto variable_list = &lookup[type_id];

    for (string item : items) {
        auto iterator = variable_list->find(item);

        if (iterator == variable_list->end()) {
            errors->push_back("Unknown " + this->classname() + " option " + item);
            continue;
        }

        iterator->second(this);
    }
}
