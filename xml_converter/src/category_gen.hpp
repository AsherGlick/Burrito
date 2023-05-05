#pragma once

#include <map>
#include <string>
#include <vector>

#include "icon_gen.hpp"
#include "parseable.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "trail_gen.hpp"
#include "waypoint.pb.h"

class XMLError;

class Category : public Parseable {
 public:
    bool default_visibility;
    std::string display_name;
    bool is_separator;
    std::string name;
    std::string tooltip_description;
    bool default_visibility_is_set = false;
    bool display_name_is_set = false;
    bool is_separator_is_set = false;
    bool name_is_set = false;
    bool tooltip_description_is_set = false;
    std::map<std::string, Category> children;
    Icon default_icon;
    Trail default_trail;

    void init_from_xml(rapidxml::xml_node<>* node, std::vector<XMLError*>* errors, std::string base_dir = "");
    virtual std::vector<std::string> as_xml() const;
    virtual std::string classname();
    bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, std::vector<XMLError*>* errors, std::string base_dir = "");
    waypoint::Category as_protobuf() const;
    void parse_protobuf(waypoint::Category proto_category);
};
