#include "category_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <type_traits>
#include <utility>

#include "attribute/bool.hpp"
#include "attribute/string.hpp"
#include "guildpoint.pb.h"
#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "string_helper.hpp"

using namespace std;

string Category::classname() {
    return "MarkerCategory";
}
void Category::init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*>* errors, XMLReaderState* state) {
    for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        bool handled = false;

        if (init_xml_attribute(attribute, errors, state)) {
            handled = true;
        }
        // Attempt to parse the attributes and throw away the results. We
        // perform this extra work here to identify if the attribute is
        // part of the particular marker, and to gather any errors there
        // might be with that attribute so they can be correctly attributed
        // to this node instead of the children that inherit the field.
        if (Icon().init_xml_attribute(attribute, errors, state)) {
            this->icon_attributes.push_back(attribute);
            handled = true;
        }
        if (Trail().init_xml_attribute(attribute, errors, state)) {
            this->trail_attributes.push_back(attribute);
            handled = true;
        }

        if (!handled) {
            errors->push_back(new XMLAttributeNameError("Unknown " + this->classname() + " attribute ", attribute));
        }
    }
}

bool Category::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*>* errors, XMLReaderState* state) {
    string attributename;
    attributename = normalize(get_attribute_name(attribute));
    if (attributename == "displayname") {
        xml_attribute_to_string(attribute, errors, state, &(this->display_name), &(this->display_name_is_set));
    }
    else if (attributename == "defaulttoggle") {
        inverted_xml_attribute_to_bool(attribute, errors, state, &(this->is_hidden), &(this->is_hidden_is_set));
    }
    else if (attributename == "isseparator") {
        xml_attribute_to_bool(attribute, errors, state, &(this->is_separator), &(this->is_separator_is_set));
    }
    else if (attributename == "id") {
        xml_attribute_to_unique_id(attribute, errors, state, &(this->menu_id), &(this->menu_id_is_set));
    }
    else if (attributename == "menuid") {
        xml_attribute_to_unique_id(attribute, errors, state, &(this->menu_id), &(this->menu_id_is_set));
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
    if (this->display_name_is_set) {
        xml_node_contents.push_back(string_to_xml_attribute("DisplayName", state, &this->display_name));
    }
    if (this->is_hidden_is_set) {
        xml_node_contents.push_back(bool_to_inverted_xml_attribute("DefaultToggle", state, &this->is_hidden));
    }
    if (this->is_separator_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("IsSeparator", state, &this->is_separator));
    }
    if (this->menu_id_is_set) {
        xml_node_contents.push_back(unique_id_to_xml_attribute("ID", state, &this->menu_id));
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

guildpoint::Category Category::as_protobuf(ProtoWriterState* state) const {
    guildpoint::Category proto_category;
    if (this->display_name_is_set) {
        std::function<void(std::string)> setter = [&proto_category](std::string val) { proto_category.set_name(val); };
        display_name_and_name_to_proto_display_name(this->display_name, state, setter, &(this->name), &(this->name_is_set));
    }
    if (this->is_hidden_is_set) {
        std::function<void(bool)> setter = [&proto_category](bool val) { proto_category.set_is_hidden(val); };
        bool_to_proto(this->is_hidden, state, setter);
    }
    if (this->is_separator_is_set) {
        std::function<void(bool)> setter = [&proto_category](bool val) { proto_category.set_is_separator(val); };
        bool_to_proto(this->is_separator, state, setter);
    }
    if (this->menu_id_is_set) {
        std::function<void(std::string)> setter = [&proto_category](std::string val) { proto_category.set_id(val); };
        unique_id_to_proto(this->menu_id, state, setter);
    }
    if (this->tooltip_description_is_set) {
        std::function<void(std::string)> setter = [&proto_category](std::string val) { proto_category.set_tip_description(val); };
        string_to_proto(this->tooltip_description, state, setter);
    }
    return proto_category;
}

void Category::parse_protobuf(guildpoint::Category proto_category, ProtoReaderState* state) {
    if (proto_category.name() != "") {
        proto_display_name_to_display_name_and_name(proto_category.name(), state, &(this->display_name), &(this->display_name_is_set), &(this->name), &(this->name_is_set));
    }
    if (proto_category.is_hidden() != 0) {
        proto_to_bool(proto_category.is_hidden(), state, &(this->is_hidden), &(this->is_hidden_is_set));
    }
    if (proto_category.is_separator() != 0) {
        proto_to_bool(proto_category.is_separator(), state, &(this->is_separator), &(this->is_separator_is_set));
    }
    if (proto_category.id() != "") {
        proto_to_unique_id(proto_category.id(), state, &(this->menu_id), &(this->menu_id_is_set));
    }
    if (proto_category.tip_description() != "") {
        proto_to_string(proto_category.tip_description(), state, &(this->tooltip_description), &(this->tooltip_description_is_set));
    }
}
