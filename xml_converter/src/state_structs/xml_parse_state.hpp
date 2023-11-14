#pragma once

#include <map>
#include <string>

class Category;

struct XMLParseState {
    std::string xml_filedir;
    std::map<std::string, Category>* marker_categories;
};
