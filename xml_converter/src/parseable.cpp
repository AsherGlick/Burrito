#include "parseable.hpp"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "rapidxml-1.13/rapidxml.hpp"
#include "string_helper.hpp"

using namespace std;


// Initalize Static variables.
uint64_t Parseable::_counter = 0;
map<string, uint64_t> Parseable::original;
map<string, map<string, void (*)(void*, rapidxml::xml_attribute<>*, vector<string>*)>> Parseable::lookup;



string Parseable::classname() {
    return "Parseable";
}


void Parseable::init_from_xml(rapidxml::xml_node<>* node, vector<string> *errors) {
    for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        if (init_xml_attribute(attribute, errors)) {}
        else {
            errors->push_back("Unknown " + this->classname() + " attribute " + string(attribute->name()));
        }
    }
}

bool Parseable::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<string> *errors) {
    const char* type_id = typeid(*this).name();
    auto variable_list = &lookup[type_id];

    string item = normalize_type_name(attribute->name());

    auto iterator = variable_list->find(item);

    if (iterator == variable_list->end()) {
        errors->push_back("Unknown " + this->classname() + " option " + item);
        return false;
    }

    iterator->second(this, attribute, errors);
    return true;
}


bool Parseable::setup_variable(
        void (*function)(void*, rapidxml::xml_attribute<>*, vector<string>*),
        vector<string> names
) {
    const char* type_id = typeid(*this).name();
    auto iterator = original.find(type_id);

    if (iterator != original.end() && iterator->second != this->_id) {
        return false;
    }
    original[type_id] = this->_id;

    // Grab a pointer to the lookup data for this subclass so we can edit it.
    map<string, void (*)(void*, rapidxml::xml_attribute<>*, vector<string>*)>* variable_list = &lookup[type_id];

    // Insert all of the names for this field, error on duplicates.
    for (auto name : names) {
        if (variable_list->count(name)) {
            throw;
        }
        (*variable_list)[normalize_type_name(name)] = function;
    }
    return false;
}
