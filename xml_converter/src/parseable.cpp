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
map<string, map<string, void (*)(void*, rapidxml::xml_attribute<>*, vector<XMLError*>*)>> Parseable::lookup;



string Parseable::classname() {
    return "Parseable";
}


void Parseable::init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*> *errors) {
    for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        if (init_xml_attribute(attribute, errors)) {}
        else {
            errors->push_back(new XMLAttributeNameError("Unknown " + this->classname() + " attribute ", attribute));
        }
    }
}

virtual bool Parseable::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors) {
    const char* type_id = typeid(*this).name();
    auto variable_list = &lookup[type_id];

    string item = normalize_type_name(get_attribute_name(attribute));

    auto iterator = variable_list->find(item);

    if (iterator == variable_list->end()) {
        return false;
    }

    iterator->second(this, attribute, errors);
    return true;
}