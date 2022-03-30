#pragma once

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

#include <string>
#include <vector>
using namespace std;


#define PARSEABLE_VAR(varname, vartype, ...) \
	vartype varname; \
	static void assign_##varname(void* obj, rapidxml::xml_attribute<>* input, vector<string> *errors) { \
		*(vartype*)obj = parse_##vartype(input, errors); \
	} \
	bool is_##varname##_set = setup_variable(assign_##varname, &varname, { __VA_ARGS__ });

#define PARSEABLE_SUBVAR(varname, subvarname, vartype, subvartype, ...) \
	static void assign_##varname##_##subvarname(void* obj, rapidxml::xml_attribute<>* input, vector<string> *errors) { \
		(*(vartype*)obj).subvarname = parse_##subvartype(input, errors); \
	}\
	bool is_##varname##_##subvarname##_set = setup_variable(assign_##varname##_##subvarname, &varname, { __VA_ARGS__ });


struct ParseableAttribute;

class Parseable {
public:
	vector<ParseableAttribute> variable_list;
	bool setup_variable(
		void (*function)(void*, rapidxml::xml_attribute<>*, vector<string>*),
		void* object,
		vector<string> names
	);


public:
	Parseable();
	virtual string classname();
	void init_from_xml(rapidxml::xml_node<>* node, vector<string> *errors);
	bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<string> *errors);
};


struct ParseableAttribute {
	void (*function)(void*, rapidxml::xml_attribute<>*, vector<string>*);
	void* object;
	vector<string> token_names;
};
