#include "{{attribute_name}}.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

{{class_name}} parse_{{attribute_name}}(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
	{{class_name}} {{attribute_name}};
{%-for n, attribute_variable in enumerate(attribute_variables)%}	
	{%-for i, value in enumerate(attribute_variable[3])%}
		{%-if i == 0 and n == 0:%}
	if (get_attribute_value(input) == "{{value}}") {
		{{attribute_name}} = {{class_name}}::{{attribute_variable[0]}};
	}
		{%- else: %}
	else if (get_attribute_value(input) == "{{value}}") {
		{{attribute_name}} = {{class_name}}::{{attribute_variable[0]}};
	}
		{%- endif %}
	{%- endfor %}
		
{%- endfor %}
	else {
		errors->push_back(new XMLAttributeValueError("Found a value that was not in the Enum", input));
        {{attribute_name}} = {{class_name}}::{{attribute_variables[0][0]}};
    }
	return {{attribute_name}};
}
