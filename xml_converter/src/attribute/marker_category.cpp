#include "marker_category.hpp"

#include <map>
#include <string>
#include <vector>

#include "../category_gen.hpp"
#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "waypoint.pb.h"

////////////////////////////////////////////////////////////////////////////////
// parse_marker_category
//
// Parses a MarkerCategory from the value of a rapidxml::xml_attribute.
////////////////////////////////////////////////////////////////////////////////
void xml_attribute_to_marker_category(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    MarkerCategory* value,
    bool* is_set) {
    value->category = get_attribute_value(input);

    const std::map<std::string, Category>* current_node = state->marker_categories;
    std::vector<std::string> category_name_segments = split(value->category, ".");
    for (size_t i = 0; i < category_name_segments.size(); i++) {
        std::string category_name_segment = category_name_segments[i];
        auto child = current_node->find(lowercase(category_name_segment));
        if (child == current_node->end()) {
            errors->push_back(new XMLAttributeValueError("Category Not Found \"" + category_name_segment + "\"", input));
            break;
        }
        current_node = &child->second.children;
    }
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// marker_category_to_xml_attribute
//
// Converts a marker category a fully qualified xml attribute string.
////////////////////////////////////////////////////////////////////////////////
std::string marker_category_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState*,
    const MarkerCategory* value) {
    return " " + attribute_name + "=\"" + value->category + "\"";
}

////////////////////////////////////////////////////////////////////////////////
// proto_to_marker_category
//
// Parses a marker category from a proto field.
////////////////////////////////////////////////////////////////////////////////
void proto_to_marker_category(
    waypoint::Category input,
    ProtoReaderState*,
    MarkerCategory* value,
    bool* is_set) {
    MarkerCategory marker_category;
    marker_category.category = input.name();
    *value = marker_category;
    *is_set = true;
}

////////////////////////////////////////////////////////////////////////////////
// to_proto_marker_category
//
// Returns a waypoint::Category so that it can be saved to proto.
////////////////////////////////////////////////////////////////////////////////
void marker_category_to_proto(
    MarkerCategory value,
    ProtoWriterState*,
    std::function<void(waypoint::Category*)> setter) {
    waypoint::Category* category = new waypoint::Category();
    category->set_name(value.category);
    setter(category);
}
