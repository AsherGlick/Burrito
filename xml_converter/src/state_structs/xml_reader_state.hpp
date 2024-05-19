#pragma once

#include <map>
#include <string>

class Category;

struct XMLReaderState {
    std::string marker_pack_root_directory;
    const std::map<std::string, Category>* marker_categories;
};
