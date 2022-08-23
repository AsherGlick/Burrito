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

bool Parseable::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *) {
    // I removed all of the offending variables. This whole section will be relooked at. 
    string item = normalize(get_attribute_name(attribute));
    return true;
}