#pragma once

#include <map>
#include <string>
#include <vector>

#include "category_gen.hpp"
#include "parseable.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "state_structs/xml_parse_state.hpp"

void parse_xml_file(
    std::string xml_filepath,
    std::map<std::string, Category>* marker_categories,
    std::vector<Parseable*>* parsed_pois);

void write_xml_file(
    std::string xml_filepath,
    std::map<std::string, Category>* marker_categories,
    std::vector<Parseable*>* parsed_pois);