#include "{{attribute_name}}.hpp"

#include <string>
#include <vector>
#include <typeinfo>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

{{class_name}} parse_{{attribute_name}}(rapidxml::xml_attribute<>* input, vector<XMLError*> *){
    {{class_name}} {{attribute_name}};
    vector<string> compound_values;
    string attributename; 
    attributename = get_attribute_name(input); 
    compound_values = split(get_attribute_value(input), ",");
    
    if (typeid(compound_values) == typeid(std::string)) {
{%-for n, attribute_variable in enumerate(attribute_variables)%}
    {%-for i, value in enumerate(attribute_variable[1])%}
        {%-if i == 0 and n == 0:%}
        if (attributename == "{{value}}") {	
            {{attribute_name}}.{{attribute_variables[n][0]}} = std::stof(get_attribute_value(input)); 
    	}
        {%- else %}
        else if (attributename == "{{value}}") {
            {{attribute_name}}.{{attribute_variables[n][0]}} = std::stof(get_attribute_value(input)); 
        }
        {%- endif %}
            
    {%- endfor %} 
{%- endfor %}
    else {
        {%-for n, attribute_variable in enumerate(attribute_variables)%}    
        {{attribute_name}}.{{attribute_variables[n][0]}} = std::stof(compound_values[{{n}}]); 
        {%- endfor %} 
        }
    }

    return {{attribute_name}};
	
}