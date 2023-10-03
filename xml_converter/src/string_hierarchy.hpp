#pragma once

#include <map>
#include <string>
#include <vector>

class StringHierarchy {
 public:
    bool in_hierarchy(
        const std::vector<std::string> &path) const;

    void add_path(
        const std::vector<std::string> &path,
        const bool include_all_chidren);

 private:
    bool _in_hierarchy(
        const std::vector<std::string> &path,
        const size_t continue_index) const;

    void _add_path(
        const std::vector<std::string> &path,
        const bool include_all_chidren,
        const size_t continue_index);

    std::map<std::string, StringHierarchy> children;
    bool all_children_included = false;
};
