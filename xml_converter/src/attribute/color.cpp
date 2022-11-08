#include "color.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

Color parse_color(rapidxml::xml_attribute<>* input, vector<XMLError*> *) {
    Color color;
    color.hex = get_attribute_value(input);
    return color;
}

string stringify_color (Color attribute_value){
    return attribute_value.hex;
}