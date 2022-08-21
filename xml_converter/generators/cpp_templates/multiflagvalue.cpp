#include "{{attribute_name}}.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

{{class_name}} parse_{{attribute_name}}(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
    {{class_name}} {{attribute_name}};
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");
{%-for template_variable in template_variables%}	
	{{attribute_name}}.{{template_variable[0]}} = false; 
{%- endfor %}
   
    for (string flag_value : flag_values) {
{%-for n, template_variable in enumerate(template_variables)%}	
	{%-for i, value in enumerate(template_variable[3])%}
		{%-if i == 0 and n == 0:%}
		if (flag_value == "{{value}}") {
			{{attribute_name}}.{{template_variable[0]}} = true; 
		}
		{%- else: %}
		else if (flag_value == "{{value}}") {
			{{attribute_name}}.{{template_variable[0]}} = true; 
		}
		{%- endif %}
	{%- endfor %}
{%- endfor %}
		else {
			errors->push_back(new XMLAttributeValueError("Found a value that was not in the class", input));
        	continue;
        }
    }
    return {{attribute_name}};
	
}
