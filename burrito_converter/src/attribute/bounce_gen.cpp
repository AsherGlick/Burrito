#include "bounce_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "guildpoint.pb.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// from_xml_attribute
//
// Reads a Bounce from an xml attribute.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Bounce::from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    Bounce* value,
    bool* is_set
) {
    Bounce bounce;
    string normalized_value = normalize(get_attribute_value(input));
    if (normalized_value == "bounce") {
        bounce = Bounce::bounce;
    }
    else if (normalized_value == "rise") {
        bounce = Bounce::rise;
    }
    else {
        errors->push_back(new XMLAttributeValueError("Found an invalid value that was not in the Enum Bounce", input));
        bounce = Bounce::bounce;
    }
    *value = bounce;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_xml_attribute
//
// Writes a Bounce to an xml attribute using the provided setter function.
////////////////////////////////////////////////////////////////////////////////
void Attribute::Bounce::to_xml_attribute(
    XMLWriterState*,
    const Bounce* value,
    std::function<void(std::string)> setter
) {
    if (*value == Bounce::bounce) {
        setter("bounce");
        return;
    }
    else if (*value == Bounce::rise) {
        setter("rise");
        return;
    }
    else {
        setter("Bounce::bounce");
    }
}
