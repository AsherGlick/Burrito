#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "category_gen.hpp"
#include "file_helper.hpp"
#include "parseable.hpp"
#include "state_structs/xml_reader_state.hpp"

std::map<Attribute::UniqueId::UniqueId, Category*> parse_xml_file(
    const MarkerPackFile& xml_filepath,
    std::map<std::string, Category>* marker_categories,
    std::vector<Parseable*>* parsed_pois
);

void write_xml_file(
    const std::string marker_pack_root_directory,
    std::map<std::string, Category>* marker_categories,
    std::vector<Parseable*>* parsed_pois
);

void cleanup_xml_files();
