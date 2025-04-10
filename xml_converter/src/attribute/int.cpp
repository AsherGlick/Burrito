#include "int.hpp"

#include <iosfwd>
#include <stdexcept>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads an int from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Int::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    int* value,
    bool* is_set
) {
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
// to_xml_attribute
//
// Converts an int a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Int::to_xml_attribute(
    XMLWriterState*,
    const int* value,
    std::function<void(std::string)> setter
) {
    setter(to_string(*value));
}

////////////////////////////////////////////////////////////////////////////////
// from_proto_field
//
// Reads an int from a proto field.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Int::from_proto_field(
    int input,
    ProtoReaderState*,
    int* value,
    bool* is_set
) {
    *value = input;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_field
//
// Writes a int to a proto using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Int::to_proto_field(
    int value,
    ProtoWriterState*,
    std::function<void(int&)> setter
) {
    setter(value);
}
