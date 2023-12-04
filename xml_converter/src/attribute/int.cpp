#include "int.hpp"

#include <iosfwd>
#include <stdexcept>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// xml_attribute_to_int
//
// Parses an int from the value of a rapidxml::xml_attribute. Adds an error
// if the value cannot be parsed properly.
////////////////////////////////////////////////////////////////////////////////
void xml_attribute_to_int(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    int* value,
    bool* is_set) {
    try {
        *value = stoi(get_attribute_value(input));
        *is_set = true;
    }
    catch (std::invalid_argument const& exception) {
        errors->push_back(new XMLAttributeValueError("Invalid integer value", input));
    }
    // TODO(#99): Do we need an out_of_range exception catch when parsing integers?
    // catch(std::out_of_range const& exception) {
    //     std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
    //     const long long ll {std::stoll(s, &pos)};
    //     std::cout << "std::stoll('" << s << "'): " << ll << "; pos: " << pos << '\n';
    // }
}

////////////////////////////////////////////////////////////////////////////////
// int_to_xml_attribute
//
// Converts an int a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
string int_to_xml_attribute(
    const string& attribute_name,
    XMLWriterState* state,
    const int* value) {
    return " " + attribute_name + "=\"" + to_string(*value) + "\"";
}

////////////////////////////////////////////////////////////////////////////////
// proto_to_int
//
// Parses an int from a proto field.
////////////////////////////////////////////////////////////////////////////////
void proto_to_int(
    int input,
    ProtoReaderState* state,
    int* value,
    bool* is_set) {
    *value = input;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// int_to_proto
//
// Writes a int to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void int_to_proto(
    int value,
    ProtoWriterState* state,
    std::function<void(int&)> setter) {
    setter(value);
}
