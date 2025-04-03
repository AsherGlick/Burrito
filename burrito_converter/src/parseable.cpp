#include "parseable.hpp"

#include <stdexcept>
#include <string>
#include <vector>

#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "state_structs/xml_writer_state.hpp"

std::string Parseable::classname() {
    return "Parseable";
}

void Parseable::init_from_xml(rapidxml::xml_node<>* node, std::vector<XMLError*>* errors, XMLReaderState* state) {
    for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        if (init_xml_attribute(attribute, errors, state)) {
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
bool Parseable::init_xml_attribute(rapidxml::xml_attribute<>*, std::vector<XMLError*>*, XMLReaderState*) {
    return false;
}

rapidxml::xml_node<char>* Parseable::as_xml(XMLWriterState*) const {
    throw std::runtime_error("error: Parseable::as_xml() should not be called");
    return nullptr;
}
