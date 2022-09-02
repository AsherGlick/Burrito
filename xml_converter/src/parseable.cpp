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


void Parseable::init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*>* errors) {
    for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        if (init_xml_attribute(attribute, errors)) {}
        else {
            errors->push_back(new XMLAttributeNameError("Unknown " + this->classname() + " attribute ", attribute));
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// Parseable::init_xml_attribute
//
// Parseable does not have any valid xml attributes as it is a base class for
// all of the other parsible classes. So just return false right away without
// doing anything.
////////////////////////////////////////////////////////////////////////////////
bool Parseable::init_xml_attribute(rapidxml::xml_attribute<>*, vector<XMLError*>*) {
    return false;
}


vector<string> Parseable::as_xml() const{
    throw std::runtime_error("error: Parseable::as_xml() should not be called");
    vector<string> result;
    return result;
}
