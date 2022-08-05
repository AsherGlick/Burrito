#pragma once

#include <string>
#include <vector>
#include <map>

#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

class Parseable {
 private:
 
 protected:
    // A stringy representation of a human readable classname. Used for errors.
    virtual string classname();

 public:
    // A default parser function to parse an entire XML node into the class.
    void init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*> *errors);

    // A default parser function to parse a single XML attribute into the class.
    virtual bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors);

};
