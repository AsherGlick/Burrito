#include "parseable.hpp"

#include <stdexcept>
#include <string>
#include <vector>

#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"

std::string Parseable::classname() {
    return "Parseable";
}

void Parseable::init_from_xml(rapidxml::xml_node<>* node, std::vector<XMLError*>* errors) {
    for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        if (init_xml_attribute(attribute, errors)) {
        }
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
bool Parseable::init_xml_attribute(rapidxml::xml_attribute<>*, std::vector<XMLError*>*) {
    return false;
}

std::vector<std::string> Parseable::as_xml() const {
    throw std::runtime_error("error: Parseable::as_xml() should not be called");
    std::vector<std::string> result;
    return result;
}

std::string Parseable::as_protobuf() const {
    throw std::runtime_error("error: Parseable::as_proto() should not be called");
    std::string result;
    return result;
}