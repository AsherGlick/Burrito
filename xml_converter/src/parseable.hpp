#pragma once

#include <string>
#include <vector>
#include <map>

#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// CLASS_PARSEABLE_VAR
//
// A macro to setup a new variable in a Parseable subclass that can be
// extracted from the attributes of an XML element. It takes in the class
// name, the name of a variable inside the class, the variable type, and a
// series of strings of names of XML attributes this field should be set from.
//
// It is suggested to #define a curried version of CLASS_PARSEABLE_VAR for each
// subclass. For Example the class SubParseable may define it as
//
//   #define PARSEABLE_VAR(...) CLASS_PARSEABLE_VAR(SubParseable, __VA_ARGS__ )
//
////////////////////////////////////////////////////////////////////////////////
#define CLASS_PARSEABLE_VAR(parseableclassname, varname, vartype, ...) \
    vartype varname; \
    static void assign_##varname(void* obj, rapidxml::xml_attribute<>* input, vector<string> *errors) { \
        (*(parseableclassname*)obj).varname = parse_##vartype(input, errors); \
        (*(parseableclassname*)obj).is_##varname##_set = true; \
    } \
    bool is_##varname##_set = setup_variable(assign_##varname, { __VA_ARGS__ });

////////////////////////////////////////////////////////////////////////////////
// CLASS_PARSEABLE_SUBVAR
//
// Similar to CLASS_PARSEABLE_VAR but instead of creating a new variable that
// can be parsed it only creates an association between an existing variable's
// member variable and a series of strings of names of XML attributes the
// member variable should be set from.
//
// It is suggested to #define a curried version of CLASS_PARSEABLE_SUBVAR for
// each subclass. For Example the class SubParseable may define it as
//
//   #define PARSEABLE_SUBVAR(...) CLASS_PARSEABLE_SUBVAR(SubParseable, __VA_ARGS__ )
//
////////////////////////////////////////////////////////////////////////////////
#define CLASS_PARSEABLE_SUBVAR(parseableclassname, varname, subvarname, vartype, subvartype, ...) \
    static void assign_##varname##_##subvarname(void* obj, rapidxml::xml_attribute<>* input, vector<string> *errors) { \
        (*(parseableclassname*)obj).varname.subvarname = parse_##subvartype(input, errors); \
        (*(parseableclassname*)obj).is_##varname##_##subvarname##_set = true; \
    } \
    bool is_##varname##_##subvarname##_set = setup_variable(assign_##varname##_##subvarname, { __VA_ARGS__ });


////////////////////////////////////////////////////////////////////////////////
// Parseable
//
// Parseable is a parent class which allows subclasses that parse a given XML
// node, to be defined quickly without duplicating schema definitions across
// multiple locations or files. To use it define a subclass of Parseable and
// then use CLASS_PARSEABLE_VAR and CLASS_PARSEABLE_SUBVAR to define all the
// attributes that can be parsed out of the node.
//
// Parseable takes advantage of many quirks of C++ to achieve a system that is
// easy to maintain and is efficient to process. All of the variables defined
// get mapped to functions linked to the subclass on first execution. These
// maps are stored in a Parseable static variable and are re-used when a second
// instance of the same subclass is instantiated.
////////////////////////////////////////////////////////////////////////////////
class Parseable {
 private:
    // A counter and ID used for identifying instances reliably.
    static uint64_t _counter;
    uint64_t _id = ++_counter;

    // A static map keeping track of the original instance id of each subclass.
    static map<string, uint64_t> original;

    // A static map keeping track of the function parser mappings for each subclass.
    static map<string, map<string, void (*)(void*, rapidxml::xml_attribute<>*, vector<string>*)>> lookup;

 protected:
    // The function responsible for registering each variable to the subclass.
    bool setup_variable(
        void (*function)(void*, rapidxml::xml_attribute<>*, vector<string>*),
        vector<string> names);

    // A stringy representation of a human readable classname. Used for errors.
    virtual string classname();

 public:
    // A default parser function to parse an entire XML node into the class.
    void init_from_xml(rapidxml::xml_node<>* node, vector<string> *errors);

    // A default parser function to parse a single XML attribute into the class.
    bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<string> *errors);
};
