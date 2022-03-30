#include "../rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>
#include "color.hpp"

using namespace std;

Color parse_Color(rapidxml::xml_attribute<>* input, vector<string> *errors) {
	Color color;
	color.hex = string(input->value());
	return color;
}
