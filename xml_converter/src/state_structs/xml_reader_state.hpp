#pragma once

#include <string>
#include <vector>

class Category;

struct XMLReaderState {
    std::string xml_filedir;
    std::vector<std::string> all_output_dirs;
};
