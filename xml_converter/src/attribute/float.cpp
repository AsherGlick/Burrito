#include "../rapidxml-1.13/rapidxml.hpp"
#include<string>
#include<vector>
#include "float.hpp"


float parse_float(rapidxml::xml_attribute<>* input, std::vector<std::string> *errors) {
	return std::stof(input->value());
}
