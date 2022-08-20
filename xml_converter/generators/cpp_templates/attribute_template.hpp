#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

{%- if type == "Enum":%}
enum {{class_name}} {
    {%- for attribute_variable in attribute_variables: %}
    {{attribute_variable[0]}},
{%- endfor %}
};
{%- else: %}
class {{class_name}} {
 public:
    {%- for attribute_variable in attribute_variables: %}
    {{attribute_variable[1]}} {{attribute_variable[0]}};
    {%- endfor %}

    virtual string classname() { return "{{class_name}}"; };
};
{%- endif %}
{{class_name}} parse_{{function_name}}(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
