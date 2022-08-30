#include "string.hpp"

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

string parse_string(rapidxml::xml_attribute<>* input, vector<XMLError*> *) {
    return get_attribute_value(input);
}

string stringify_string(string attribute_value){
    return attribute_value;
}