#include "{{attribute_name}}.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

{{class_name}} parse_{{attribute_name}}(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
    {{class_name}} {{attribute_name}};
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");
{%-for attribute_variable in attribute_variables%}	
	{{attribute_name}}.{{attribute_variable.attribute_name}} = false; 
{%- endfor %}
   
    for (string flag_value : flag_values) {
{%-for n, attribute_variable in enumerate(attribute_variables)%}	
	{%-for i, value in enumerate(attribute_variable.xml_fields)%}
		{%-if i == 0 and n == 0:%}
		if (flag_value == "{{value}}") {
			{{attribute_name}}.{{attribute_variable.attribute_name}} = true; 
		}
		{%- else: %}
		else if (flag_value == "{{value}}") {
			{{attribute_name}}.{{attribute_variable.attribute_name}} = true; 
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
