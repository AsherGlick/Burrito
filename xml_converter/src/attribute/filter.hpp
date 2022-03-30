#pragma once

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include <string>
#include <vector>
#include <map>

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// FILTER_ITEM
//
// The FILTER_ITEM macro takes a name of a flag variable and a series of values
// for what strings should trigger that flag. It useese setup_variable to
// construct a list of all the flags that can be set for a given class on
// runtime.
//
// TODO: Because everything in `Filter` is a bool, unlike how `Parseable` works
// we might not even need to store a function pointer and instead 
// Filter.variable_list could be a map<string, bool*> and we use one generic
// function to set all the flags.
////////////////////////////////////////////////////////////////////////////////
#define FILTER_ITEM(name, ...) \
	static void assign_##name(bool* obj) { \
		*obj = true; \
	} \
	bool name = setup_variable(assign_##name, &name, { __VA_ARGS__ });



////////////////////////////////////////////////////////////////////////////////
// RemoteCall
//
// RemoteCall is a struct used to store the information needed to set flags
// while we are parsing the input.
//
// TODO: Because everything in `Filter` is a bool, unlike how `Parseable` works
// we might not even need to store a function pointer and instead 
// Filter.variable_list could be a map<string, bool*> and we use one generic
// function to set all the flags.
////////////////////////////////////////////////////////////////////////////////
struct RemoteCall {
	void (*function)(bool*);
	bool* object;
};


////////////////////////////////////////////////////////////////////////////////
// Filter
//
// Filter is ment to be a base class for various attributes where one or more
// options can be selected. Typically these function as filters for whatever
// element they are associated with. For example a marker or path may be hidden
// or shown base on what class a user it when the path is relevent to only
// one class.
////////////////////////////////////////////////////////////////////////////////
class Filter {
public:
	map<string, RemoteCall> variable_list;
	bool setup_variable(void (*function)(bool*), bool*, vector<string> names);
	void parse(rapidxml::xml_attribute<>* input, vector<string> *errors);
	virtual string classname() { return "Filter"; }
};

