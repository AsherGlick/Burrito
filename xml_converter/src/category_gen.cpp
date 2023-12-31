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
void Category::init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*>* errors, XMLReaderState* state) {
    for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        bool is_icon_value = this->default_icon.init_xml_attribute(attribute, errors, state);
        bool is_trail_value = this->default_trail.init_xml_attribute(attribute, errors, state);

        if (init_xml_attribute(attribute, errors, state)) {
        }
        else if (is_icon_value || is_trail_value) {
        }
        else {
            errors->push_back(new XMLAttributeNameError("Unknown " + this->classname() + " attribute ", attribute));
        }
    }
}

bool Category::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*>* errors, XMLReaderState* state) {
    string attributename;
    attributename = normalize(get_attribute_name(attribute));
    if (attributename == "defaulttoggle") {
        xml_attribute_to_bool(attribute, errors, state, &(this->default_visibility), &(this->default_visibility_is_set));
    }
    else if (attributename == "displayname") {
        xml_attribute_to_string(attribute, errors, state, &(this->display_name), &(this->display_name_is_set));
    }
    else if (attributename == "isseparator") {
        xml_attribute_to_bool(attribute, errors, state, &(this->is_separator), &(this->is_separator_is_set));
    }
    else if (attributename == "name") {
        xml_attribute_to_string(attribute, errors, state, &(this->name), &(this->name_is_set));
    }
    else if (attributename == "tipdescription") {
        xml_attribute_to_string(attribute, errors, state, &(this->tooltip_description), &(this->tooltip_description_is_set));
    }
    else {
        return false;
    }
    return true;
}

vector<string> Category::as_xml(XMLWriterState* state) const {
    vector<string> xml_node_contents;
    xml_node_contents.push_back("<MarkerCategory ");
    if (this->default_visibility_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("DefaultToggle", state, &this->default_visibility));
    }
    if (this->display_name_is_set) {
        xml_node_contents.push_back(string_to_xml_attribute("DisplayName", state, &this->display_name));
    }
    if (this->is_separator_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("IsSeparator", state, &this->is_separator));
    }
    if (this->name_is_set) {
        xml_node_contents.push_back(string_to_xml_attribute("Name", state, &this->name));
    }
    if (this->tooltip_description_is_set) {
        xml_node_contents.push_back(string_to_xml_attribute("TipDescription", state, &this->tooltip_description));
    }
    xml_node_contents.push_back(">\n");

    for (const auto& [key, val] : this->children) {
        string text;
        for (const auto& s : val.as_xml(state)) {
            text += s;
        }

        xml_node_contents.push_back("\t" + text);
    }

    xml_node_contents.push_back("</MarkerCategory>\n");
    return xml_node_contents;
}

waypoint::Category Category::as_protobuf(ProtoWriterState* state) const {
    waypoint::Category proto_category;
    if (this->default_visibility_is_set) {
        std::function<void(bool)> setter = [&proto_category](bool val) { proto_category.set_default_visibility(val); };
        bool_to_proto(this->default_visibility, state, setter);
    }
    if (this->display_name_is_set) {
        std::function<void(std::string)> setter = [&proto_category](std::string val) { proto_category.set_name(val); };
        display_name_and_name_to_proto_display_name(this->display_name, state, setter, &(this->name), &(this->name_is_set));
    }
    if (this->is_separator_is_set) {
        std::function<void(bool)> setter = [&proto_category](bool val) { proto_category.set_is_separator(val); };
        bool_to_proto(this->is_separator, state, setter);
    }
    if (this->name_is_set) {
        std::function<void(std::string)> setter = [&proto_category](std::string val) { proto_category.set_name(val); };
        do_nothing(this->name, state, setter);
    }
    if (this->tooltip_description_is_set) {
        std::function<void(std::string)> setter = [&proto_category](std::string val) { proto_category.set_tip_description(val); };
        string_to_proto(this->tooltip_description, state, setter);
    }
    return proto_category;
}

void Category::parse_protobuf(waypoint::Category proto_category, ProtoReaderState* state) {
    if (proto_category.default_visibility() != 0) {
        proto_to_bool(proto_category.default_visibility(), state, &(this->default_visibility), &(this->default_visibility_is_set));
    }
    if (proto_category.name() != "") {
        proto_display_name_to_display_name_and_name(proto_category.name(), state, &(this->display_name), &(this->display_name_is_set), &(this->name), &(this->name_is_set));
    }
    if (proto_category.is_separator() != 0) {
        proto_to_bool(proto_category.is_separator(), state, &(this->is_separator), &(this->is_separator_is_set));
    }
    if (proto_category.name() != "") {
        do_nothing(proto_category.name(), state, &(this->name), &(this->name_is_set));
    }
    if (proto_category.tip_description() != "") {
        proto_to_string(proto_category.tip_description(), state, &(this->tooltip_description), &(this->tooltip_description_is_set));
    }
}
