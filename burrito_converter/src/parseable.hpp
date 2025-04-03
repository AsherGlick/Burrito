#pragma once

#include <string>
#include <vector>

#include "rapidxml-1.13/rapidxml.hpp"
#include "state_structs/xml_reader_state.hpp"
#include "state_structs/xml_writer_state.hpp"

class XMLError;

class Parseable {
 public:
    // A stringy representation of a human readable classname. Used for errors.
    virtual std::string classname();

    // A default parser function to parse an entire XML node into the class.
    void init_from_xml(rapidxml::xml_node<>* node, std::vector<XMLError*>* errors, XMLReaderState* state);

    // A default parser function to parse a single XML attribute into the class.
    virtual bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, std::vector<XMLError*>* errors, XMLReaderState*);

    virtual rapidxml::xml_node<char>* as_xml(XMLWriterState* state) const;
};
