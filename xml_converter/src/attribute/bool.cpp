#include "bool.hpp"

#include <stdexcept>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

bool parse_bool(rapidxml::xml_attribute<>* input, vector<string> *errors) {
    if (string(input->value()) == "0" || string(input->value()) == "false") {
        return false;
    }
    else if (string(input->value()) == "1" || string(input->value()) == "true") {
        return true;
    }
    else {
        errors->push_back("Found a boolean value that was not a '1', '0', 'true', or 'false'");
        return false;
    }
}
