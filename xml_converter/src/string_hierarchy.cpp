#include "string_hierarchy.hpp"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// in_hierarchy
//
// Returns if the given path is in the hierarchy or not
////////////////////////////////////////////////////////////////////////////////
bool StringHierarchy::in_hierarchy(
    const std::vector<std::string> &path) {
    return this->_in_hierarchy(path, 0);
}

////////////////////////////////////////////////////////////////////////////////
// _in_hirearchy
//
// Recursive helper function to traverse the tree and determine if a path is
// or is not in the current hierarchy.
////////////////////////////////////////////////////////////////////////////////
bool StringHierarchy::_in_hierarchy(
    const std::vector<std::string> &path,
    const size_t continue_index) {
    // If all children of this hierarchy node are included then this path exists.
    if (this->all_children_included) {
        return true;
    }

    // If this is the end of the path then the path exists.
    if (continue_index >= path.size()) {
        return true;
    }

    auto iterator = this->children.find(path[continue_index]);
    if (iterator == this->children.end()) {
        return false;
    }

    return iterator->second._in_hierarchy(path, continue_index + 1);
}

////////////////////////////////////////////////////////////////////////////////
// add_path
//
// Adds a new path to the StringHierarchy.
////////////////////////////////////////////////////////////////////////////////
void StringHierarchy::add_path(
    const std::vector<std::string> &path,
    const bool include_all_chidren) {
    return this->_add_path(
        path,
        include_all_chidren,
        0);
}

////////////////////////////////////////////////////////////////////////////////
// _add_path
//
// Recursive helper function to add new paths into the string hierarchy.
////////////////////////////////////////////////////////////////////////////////
void StringHierarchy::_add_path(
    const std::vector<std::string> &path,
    const bool include_all_chidren,
    const size_t continue_index) {
    // If all children are already included no need to specify any more children.
    if (this->all_children_included) {
        return;
    }

    // End of the path, if all children are included clear out more specific children.
    if (continue_index >= path.size()) {
        if (include_all_chidren) {
            this->all_children_included = true;
            this->children.clear();
        }
        return;
    }

    this->children[path[continue_index]]._add_path(
        path,
        include_all_chidren,
        continue_index + 1);
}
