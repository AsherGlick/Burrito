#include "{{attribute_name}}.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

{{class_name}} parse_{{attribute_name}}(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
	{{class_name}} {{attribute_name}};
	string normalized_value = normalize(get_attribute_value(input));
{%-for n, attribute_variable in enumerate(attribute_variables)%}	
	{%-for i, value in enumerate(attribute_variable.xml_fields)%}
		{%-if i == 0 and n == 0:%}
	if (normalized_value == "{{value}}") {
		{{attribute_name}} = {{class_name}}::{{attribute_variable.attribute_name}};
	}
		{%- else: %}
	else if (normalized_value == "{{value}}") {
		{{attribute_name}} = {{class_name}}::{{attribute_variable.attribute_name}};
	}
		{%- endif %}
	{%- endfor %}
		
{%- endfor %}
	else {
		errors->push_back(new XMLAttributeValueError("Found an invalid value that was not in the Enum {{class_name}}", input));
        {{attribute_name}} = {{class_name}}::{{attribute_variables[0].attribute_name}};
    }
	return {{attribute_name}};
}
