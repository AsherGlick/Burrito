#pragma once

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "category_gen.hpp"
#include "file_helper.hpp"
#include "guildpoint.pb.h"
#include "parseable.hpp"
#include "string_hierarchy.hpp"

std::map<Attribute::UniqueId::UniqueId, Category*> read_protobuf_file(
    const MarkerPackFile& proto_filepath,
    std::map<std::string, Category>* marker_categories,
    std::vector<Parseable*>* parsed_pois
);

void write_protobuf_file(
    const std::string& marker_pack_root_directory,
    const StringHierarchy& category_filter,
    const std::map<std::string, Category>* marker_categories,
    const std::vector<Parseable*>* parsed_pois
);

void write_protobuf_file_per_map_id(
    const std::string& marker_pack_root_directory,
    const StringHierarchy& category_filter,
    const std::map<std::string, Category>* marker_categories,
    const std::vector<Parseable*>* parsed_pois
);

void write_protobuf_file_per_category(
    const std::string& marker_pack_root_directory,
    const int split_by_category_depth,
    const std::map<std::string, Category>* marker_categories,
    const std::vector<Parseable*>* parsed_pois
);
