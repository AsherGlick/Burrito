#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "category_gen.hpp"
#include "file_helper.hpp"
#include "parseable.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "state_structs/xml_reader_state.hpp"

std::set<std::string> parse_xml_file(
    const MarkerPackFile& xml_filepath,
    std::map<std::string, Category>* marker_categories,
    std::vector<Parseable*>* parsed_pois);

void write_xml_file(
    const std::string marker_pack_root_directory,
    std::map<std::string, Category>* marker_categories,
    std::vector<Parseable*>* parsed_pois);
