#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "category_gen.hpp"
#include "parseable.hpp"
#include "waypoint.pb.h"

void read_protobuf_file(
    std::string proto_filepath,
    std::map<std::string, Category>* marker_categories,
    std::vector<Parseable*>* parsed_pois);

void write_protobuf_file(
    std::string proto_directory,
    std::map<std::string, Category>* marker_categories,
    std::vector<Parseable*>* parsed_pois);
