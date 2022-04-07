#include "bool.hpp"

#include <stdexcept>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../rapid_helpers.hpp"

using namespace std;

bool parse_bool(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors) {
    if (get_attribute_value(input) == "0" || get_attribute_value(input) == "false") {
        return false;
    }
    else if (get_attribute_value(input) == "1" || get_attribute_value(input) == "true") {
        return true;
    }
    else {
        errors->push_back(new XMLAttributeValueError("Found a boolean value that was not a '1', '0', 'true', or 'false'", input));
        return false;
    }
}
