#include "category_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <type_traits>
#include <utility>

#include "attribute/bool.hpp"
#include "attribute/string.hpp"
#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "string_helper.hpp"
#include "waypoint.pb.h"

using namespace std;

string Category::classname() {
    return "MarkerCategory";
}
void Category::init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*>* errors, string base_dir) {
    for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        bool is_icon_value = this->default_icon.init_xml_attribute(attribute, errors);
        bool is_trail_value = this->default_trail.init_xml_attribute(attribute, errors);

        if (init_xml_attribute(attribute, errors, base_dir)) {
        }
        else if (is_icon_value || is_trail_value) {
        }
        else {
            errors->push_back(new XMLAttributeNameError("Unknown " + this->classname() + " attribute ", attribute));
        }
    }
}

bool Category::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*>* errors, string base_dir) {
    string attributename;
    attributename = normalize(get_attribute_name(attribute));
    if (attributename == "defaulttoggle") {
        this->default_visibility = parse_bool(attribute, errors);
        this->default_visibility_is_set = true;
    }
    else if (attributename == "displayname") {
        this->display_name = parse_string(attribute, errors);
        this->display_name_is_set = true;
    }
    else if (attributename == "isseparator") {
        this->is_separator = parse_bool(attribute, errors);
        this->is_separator_is_set = true;
    }
    else if (attributename == "name") {
        this->name = parse_string(attribute, errors);
        this->name_is_set = true;
    }
    else if (attributename == "tipdescription") {
        this->tooltip_description = parse_string(attribute, errors);
        this->tooltip_description_is_set = true;
    }
    else {
        return false;
    }
    return true;
}

vector<string> Category::as_xml() const {
    vector<string> xml_node_contents;
    xml_node_contents.push_back("<MarkerCategory ");
    if (this->default_visibility_is_set) {
        xml_node_contents.push_back(" DefaultToggle=\"" + stringify_bool(this->default_visibility) + "\"");
    }
    if (this->display_name_is_set) {
        xml_node_contents.push_back(" DisplayName=\"" + stringify_string(this->display_name) + "\"");
    }
    if (this->is_separator_is_set) {
        xml_node_contents.push_back(" IsSeparator=\"" + stringify_bool(this->is_separator) + "\"");
    }
    if (this->name_is_set) {
        xml_node_contents.push_back(" Name=\"" + stringify_string(this->name) + "\"");
    }
    if (this->tooltip_description_is_set) {
        xml_node_contents.push_back(" TipDescription=\"" + stringify_string(this->tooltip_description) + "\"");
    }
    xml_node_contents.push_back(">\n");

    for (const auto& [key, val] : this->children) {
        string text;
        for (const auto& s : val.as_xml()) {
            text += s;
        }

        xml_node_contents.push_back("\t" + text);
    }

    xml_node_contents.push_back("</MarkerCategory>\n");
    return xml_node_contents;
}

waypoint::Category Category::as_protobuf() const {
    waypoint::Category proto_category;
    if (this->default_visibility_is_set) {
        proto_category.set_default_visibility(to_proto_bool(this->default_visibility));
    }
    if (this->display_name_is_set) {
        proto_category.set_display_name(to_proto_string(this->display_name));
    }
    if (this->is_separator_is_set) {
        proto_category.set_is_separator(to_proto_bool(this->is_separator));
    }
    if (this->name_is_set) {
        proto_category.set_name(to_proto_string(this->name));
    }
    if (this->tooltip_description_is_set) {
        proto_category.set_tip_description(to_proto_string(this->tooltip_description));
    }
    return proto_category;
}

void Category::parse_protobuf(waypoint::Category proto_category) {
    if (proto_category.default_visibility() != 0) {
        this->default_visibility = from_proto_bool(proto_category.default_visibility());
        this->default_visibility_is_set = true;
    }
    if (proto_category.display_name() != "") {
        this->display_name = from_proto_string(proto_category.display_name());
        this->display_name_is_set = true;
    }
    if (proto_category.is_separator() != 0) {
        this->is_separator = from_proto_bool(proto_category.is_separator());
        this->is_separator_is_set = true;
    }
    if (proto_category.name() != "") {
        this->name = from_proto_string(proto_category.name());
        this->name_is_set = true;
    }
    if (proto_category.tip_description() != "") {
        this->tooltip_description = from_proto_string(proto_category.tip_description());
        this->tooltip_description_is_set = true;
    }
}
