#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

class {{class_name}} {
 public:
        {%- for attribute_variable in attribute_variables: %}
        bool {{attribute_variable}};
        {%- endfor %}

        virtual string classname() { return "{{class_name}}"; };
};

{{class_name}} parse_{{class_name}}(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
