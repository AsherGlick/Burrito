#pragma once

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <map>

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// CLASS_FILTER_ITEM
//
// The CLASS_FILTER_ITEM macro takes the name of a class, the name of a
// variable inside that class, and a series of strings that should cause that
// variable to be set. It uses setup_variable to construct a list of all the 
// flags that can be set for a given Filter subclass at runtime. It has no
// references to the object itself so that it can be instance agnostic and
// re-used when subsequent classes are initialized.
//
// It is suggested to #define a curried version of CLASS_FILTER_ITEM for each
// subclass. For example the class SubFilter may define it as:
//
//   #define FILTER_ITEM(...) CLASS_FILTER_ITEM(SubFilter, __VA_ARGS__ )
//
////////////////////////////////////////////////////////////////////////////////
#define CLASS_FILTER_ITEM(filtername, name, ...) \
	bool name; \
	static void enable_##name(void* obj) { \
		(*(filtername*)obj).name = true; \
	} \
	bool name##_setup = setup_variable(enable_##name, &name, { __VA_ARGS__ });

////////////////////////////////////////////////////////////////////////////////
// Filter
//
// Filter is meant to be a base class for various attributes where one or more
// options can be selected. Typically these function as filters for whatever
// element they are associated with. For example a marker or path may be hidden
// or shown base on what class a user it when the path is relevant to only
// one class.
////////////////////////////////////////////////////////////////////////////////
class Filter {
	static uint64_t _counter;
	uint64_t _id = ++_counter;

public:
	bool setup_variable(void (*function)(void* filter_object), void* object, vector<string> names);

	void parse(rapidxml::xml_attribute<>* input, vector<string> *errors);
	virtual string classname() { return "Filter"; }
};

