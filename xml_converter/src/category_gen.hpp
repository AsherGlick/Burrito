#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "attribute/unique_id.hpp"
#include "icon_gen.hpp"
#include "parseable.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "state_structs/xml_reader_state.hpp"
#include "trail_gen.hpp"
#include "waypoint.pb.h"

class XMLError;

class Category : public Parseable {
 public:
    bool default_visibility;
    std::string display_name;
    bool is_separator;
    UniqueId menu_id;
    std::string name;
    std::string tooltip_description;
    bool default_visibility_is_set = false;
    bool display_name_is_set = false;
    bool is_separator_is_set = false;
    bool menu_id_is_set = false;
    bool name_is_set = false;
    bool tooltip_description_is_set = false;
    std::map<std::string, Category> children;
    Icon default_icon;
    Trail default_trail;
    Category* parent;

    void init_from_xml(rapidxml::xml_node<>* node, std::vector<XMLError*>* errors, XMLReaderState* state);
    virtual std::vector<std::string> as_xml(XMLWriterState* state) const;
    virtual std::string classname();
    bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, std::vector<XMLError*>* errors, XMLReaderState* state);
    waypoint::Category as_protobuf(ProtoWriterState* state) const;
    void parse_protobuf(waypoint::Category proto_category, ProtoReaderState* state);
    void apply_underlay(const Category& underlay);
    void apply_overlay(const Category& overlay);
};
