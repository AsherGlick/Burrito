#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "category_gen.hpp"
#include "parseable.hpp"
#include "string_hierarchy.hpp"
#include "waypoint.pb.h"

void read_protobuf_file(
    std::string proto_filepath,
    std::map<std::string, Category>* marker_categories,
    std::vector<Parseable*>* parsed_pois);

void write_protobuf_file(
    const std::string& proto_directory,
    const StringHierarchy& category_filter,
    const std::map<std::string, Category>* marker_categories,
    const std::vector<Parseable*>* parsed_pois);

void write_protobuf_file_per_map_id(
    const std::string& proto_directory,
    const StringHierarchy& category_filter,
    const std::map<std::string, Category>* marker_categories,
    const std::vector<Parseable*>* parsed_pois);
