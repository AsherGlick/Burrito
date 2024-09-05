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
    std::string display_name;
    bool hide_category;
    bool is_separator;
    UniqueId menu_id;
    std::string name;
    std::string tooltip_description;
    bool display_name_is_set = false;
    bool hide_category_is_set = false;
    bool is_separator_is_set = false;
    bool menu_id_is_set = false;
    bool name_is_set = false;
    bool tooltip_description_is_set = false;
    std::map<std::string, Category> children;
    std::vector<rapidxml::xml_attribute<>*> icon_attributes;
    std::vector<rapidxml::xml_attribute<>*> trail_attributes;
    Category* parent;

    void init_from_xml(rapidxml::xml_node<>* node, std::vector<XMLError*>* errors, XMLReaderState* state);
    virtual std::vector<std::string> as_xml(XMLWriterState* state) const;
    virtual std::string classname();
    bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, std::vector<XMLError*>* errors, XMLReaderState* state);
    waypoint::Category as_protobuf(ProtoWriterState* state) const;
    void parse_protobuf(waypoint::Category proto_category, ProtoReaderState* state);
};
