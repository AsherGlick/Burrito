#include "color.hpp"

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

Color parse_Color(rapidxml::xml_attribute<>* input, vector<string> *) {
    Color color;
    color.hex = string(input->value());
    return color;
}
