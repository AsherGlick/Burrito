#include "bool.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// parse_bool
//
// Parses a bool from the value of a rapidxml::xmlattribute. "true" or "1" are
// evaluated as `true`. 'false' or '0' are evaluated as `false`. Everything is
// is also evaluated as false but appends an error to the errors vector.
////////////////////////////////////////////////////////////////////////////////
bool parse_bool(rapidxml::xml_attribute<>* input, vector<XMLError*>* errors) {
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

////////////////////////////////////////////////////////////////////////////////
// bool_to_xml_attribute
//
// Converts a bool into a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
string bool_to_xml_attribute(const string& attribute_name, const bool* value) {
    if (value) {
        return " " + attribute_name + "=\"true\"";
    }
    else {
        return " " + attribute_name + "=\"false\"";
    }
}
