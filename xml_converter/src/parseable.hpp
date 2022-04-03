#pragma once

#include <string>
#include <vector>
#include <map>

#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;


#define CLASS_PARSEABLE_VAR(parseableclassname, varname, vartype, ...) \
    vartype varname; \
    static void assign_##varname(void* obj, rapidxml::xml_attribute<>* input, vector<string> *errors) { \
        (*(parseableclassname*)obj).varname = parse_##vartype(input, errors); \
    } \
    bool is_##varname##_set = setup_variable(assign_##varname, { __VA_ARGS__ });

#define CLASS_PARSEABLE_SUBVAR(parseableclassname, varname, subvarname, vartype, subvartype, ...) \
    static void assign_##varname##_##subvarname(void* obj, rapidxml::xml_attribute<>* input, vector<string> *errors) { \
        (*(parseableclassname*)obj).varname.subvarname = parse_##subvartype(input, errors); \
    }\
    bool is_##varname##_##subvarname##_set = setup_variable(assign_##varname##_##subvarname, { __VA_ARGS__ });


class Parseable {
    static uint64_t _counter;
    uint64_t _id = ++_counter;

    static map<string, uint64_t> original;
    static map<string, map<string, void (*)(void*, rapidxml::xml_attribute<>*, vector<string>*)>> lookup;

 public:
    bool setup_variable(
        void (*function)(void*, rapidxml::xml_attribute<>*, vector<string>*),
        vector<string> names);

    virtual string classname();
    void init_from_xml(rapidxml::xml_node<>* node, vector<string> *errors);
    bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<string> *errors);
};
