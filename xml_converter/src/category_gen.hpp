#pragma once

#include "icon_gen.hpp"

#include <map>
#include <string>
#include <vector>

#include "parseable.hpp"
#include "trail_gen.hpp"

#include "rapidxml-1.13/rapidxml.hpp"

class XMLError;

class Category: public Parseable {
    public:
        bool default_visibility;
        bool default_visibility_is_set = false;
        std::string display_name;
        bool display_name_is_set = false;
        bool is_separator;
        bool is_separator_is_set = false;
        std::string name;
        bool name_is_set = false;
        std::string tooltip_name;
        bool tooltip_name_is_set = false;
        std::map<std::string, Category> children;
        Icon default_icon;
        Trail default_trail;

        void init_from_xml(rapidxml::xml_node<>* node, std::vector<XMLError*> *errors);
        virtual std::string classname();
        bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, std::vector<XMLError*> *errors);
        virtual std::vector<std::string> as_xml() const;
};