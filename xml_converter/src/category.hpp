#pragma once

#include <map>
#include <string>
#include <vector>

#include "icon.hpp"
#include "parseable.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "trail.hpp"



using namespace std;

// #define PARSEABLE_VAR(...) CLASS_PARSEABLE_VAR(Icon, __VA_ARGS__ )
// #define PARSEABLE_SUBVAR(...) CLASS_PARSEABLE_SUBVAR(Icon, __VA_ARGS__ )

class Category: public Parseable {
	private: 
	
		bool default_visibility;
	
		string display_name;
	
		bool is_seperator;
	
		string name;
	
		string tooltip_name;
	
	
	  map<string, Category> children;
    Icon default_icon;
    Trail default_trail;
    
	virtual string classname();
};

// #undef PARSEABLE_VAR
// #undef PARSEABLE_SUBVAR