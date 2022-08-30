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
        bool default_visibility_is_set = false;
        string display_name;
        bool display_name_is_set = false;
        bool is_separator;
        bool is_separator_is_set = false;
        string name;
        bool name_is_set = false;
        string tooltip_name;
        bool tooltip_name_is_set = false;
        map<string, Category> children;
        Icon default_icon;
        Trail default_trail;

        void init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*> *errors);
        virtual string classname();
        bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors);
        virtual vector<string> as_xml() const;
};