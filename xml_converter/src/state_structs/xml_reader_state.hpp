#pragma once

#include <map>
#include <string>

class Category;

struct XMLReaderState {
    std::string marker_pack_root_directory;
    std::map<std::string, Category>* marker_categories;
};
