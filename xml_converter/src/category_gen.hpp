#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>
#include "parseable.hpp"
#include <map>
#include "icon_gen.hpp"
#include "trail_gen.hpp"

#include "attribute/bool.hpp"
#include "attribute/string.hpp"
using namespace std;

class Category: public Parseable {
    public:
        bool default_visibility;
        string display_name;
        bool is_separator;
        string name;
        string tooltip_name;
        bool default_visibility_is_set = false;
        bool display_name_is_set = false;
        bool is_separator_is_set = false;
        bool name_is_set = false;
        bool tooltip_name_is_set = false;
        map<string, Category> children;
        Icon default_icon;
        Trail default_trail;

        virtual vector<string> as_xml() const;
        void init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*> *errors);
        virtual string classname();
        bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors);
};