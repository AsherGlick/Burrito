#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>
#include "parseable.hpp"
{%- if cpp_class == "Category": %}
#include <map>
#include "icon_gen.hpp"
#include "trail_gen.hpp"
{%- elif cpp_class == "Trail": %}
#include <string.h>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string_view>
#include "rapidxml-1.13/rapidxml_print.hpp"
{%- endif %}
{% for cpp_include_path in cpp_include_paths %}
#include "attribute/{{cpp_include_path}}.hpp"
{%- endfor %}
using namespace std;

class {{cpp_class}}: public Parseable {
    public:
        {%- for attribute_variable in attribute_variables: %}
        {{attribute_variable.cpp_type}} {{attribute_variable.attribute_name}};
        {%- endfor %}
        {%- for attribute_variable in attribute_variables: %}
        bool {{attribute_variable.attribute_name}}_is_set = false;
        {%- endfor %}

        {%- if cpp_class == "Category": %}
        map<string, Category> children;
        Icon default_icon;
        Trail default_trail;

        void init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*> *errors);
        {%- endif %}
        virtual vector<string> as_xml() const;
        virtual string classname();
        bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors);
        {%-if attributes_of_type_marker_category %}
        bool validate_attributes_of_type_marker_category();
        {%- endif %}
};
