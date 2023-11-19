#pragma once

#include <map>
#include <string>

class Category;

struct XMLReaderState {
    std::string xml_filedir;
    std::map<std::string, Category>* marker_categories;
};
