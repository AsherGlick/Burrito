#pragma once

#include <map>
#include <string>
#include <vector>

#include "icon.hpp"
#include "parseable.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "trail.hpp"

using namespace std;



#define PARSEABLE_VAR(...) CLASS_PARSEABLE_VAR(Category, __VA_ARGS__ )
#define PARSEABLE_SUBVAR(...) CLASS_PARSEABLE_SUBVAR(Category, __VA_ARGS__ )

class Category: public Parseable {
 public:
    // https://blishhud.com/docs/markers/attributes/defaulttoggle
    PARSEABLE_VAR(default_toggle, bool, "DefaultToggle")

    // https://blishhud.com/docs/markers/attributes/displayname
    PARSEABLE_VAR(display_name, string, "DisplayName")

    // https://blishhud.com/docs/markers/attributes/isseparator
    PARSEABLE_VAR(is_separator, bool, "IsSeparator")

    // Not Documented on blishhud
    PARSEABLE_VAR(name, string, "Name")


    map<string, Category> children;
    Icon default_icon;
    Trail default_trail;


    virtual string classname();
    void init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*> *errors);
};

#undef PARSEABLE_VAR
#undef PARSEABLE_SUBVAR
