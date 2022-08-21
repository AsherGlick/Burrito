#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>
#include "parseable.hpp"
{%- if cpp_class == "Category": %}
#include <map>
#include "icon.hpp"
#include "trail.hpp"
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
        {{attribute_variable[1]}} {{attribute_variable[0]}};

        {%- endfor %}

        {%- if cpp_class == "Category": %}
        map<string, Category> children;
        Icon default_icon;
        Trail default_trail;

        void init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*> *errors);
        {%- endif %}
        virtual string classname();
        bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors);
        {%-if attributes_of_type_marker_category %}
        bool validate_attributes_of_type_marker_category();
        {%- endif %}
};