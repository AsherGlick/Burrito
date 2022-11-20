#include "int.hpp"

#include <iosfwd>
#include <stdexcept>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// parse_int
//
// Parses an int from the value of a rapidxml::xml_attribute. Adds an error
// if the value cannot be parsed properly.
////////////////////////////////////////////////////////////////////////////////
int parse_int(rapidxml::xml_attribute<>* input, vector<XMLError*>* errors) {
    try {
        return stoi(get_attribute_value(input));
    }
    catch (std::invalid_argument const& exception) {
        errors->push_back(new XMLAttributeValueError("Invalid integer value", input));
        return 0;
    }
    // TODO(#99): Do we need an out_of_range exception catch when parsing integers?
    // catch(std::out_of_range const& exception) {
    //     std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
    //     const long long ll {std::stoll(s, &pos)};
    //     std::cout << "std::stoll('" << s << "'): " << ll << "; pos: " << pos << '\n';
    // }
}

////////////////////////////////////////////////////////////////////////////////
// stringify_int
//
// Converts an int into a stringy value so that it can be saved to xml.
////////////////////////////////////////////////////////////////////////////////
string stringify_int(int attribute_value) {
    return to_string(attribute_value);
}
