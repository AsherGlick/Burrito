#include "string_hierarchy.hpp"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// in_hierarchy
//
// Returns if a particular node exists at the top level of the hirearchy.
////////////////////////////////////////////////////////////////////////////////
bool StringHierarchy::in_hierarchy(
    const std::string &node) const {
    if (this->all_children_included) {
        return true;
    }

    auto iterator = this->children.find(node);
    if (iterator == this->children.end()) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// in_hierarchy
//
// Returns if the given path is in the hierarchy or not
////////////////////////////////////////////////////////////////////////////////
bool StringHierarchy::in_hierarchy(
    const std::vector<std::string> &path) const {
    return this->_in_hierarchy(path, 0);
}

////////////////////////////////////////////////////////////////////////////////
// in_hierarchy
//
// An explicit version of in_hierarchy that takes an initalizer list to prevent
// ambiguity between the vector and string overloads of the function.
////////////////////////////////////////////////////////////////////////////////
bool StringHierarchy::in_hierarchy(
    const std::initializer_list<std::string> input) const {
    std::vector<std::string> vec;
    vec.insert(vec.end(), input.begin(), input.end());
    return this->in_hierarchy(vec);
}

////////////////////////////////////////////////////////////////////////////////
// _in_hirearchy
//
// Recursive helper function to traverse the tree and determine if a path is
// or is not in the current hierarchy.
////////////////////////////////////////////////////////////////////////////////
bool StringHierarchy::_in_hierarchy(
    const std::vector<std::string> &path,
    const size_t continue_index) const {
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
// sub_hierarchy
//
// A helper function to grab a sub hierarchy one level down from the top.
////////////////////////////////////////////////////////////////////////////////
const StringHierarchy *StringHierarchy::sub_hierarchy(
    const std::string &node) const {
    if (this->all_children_included) {
        return this;
    }

    auto iterator = this->children.find(node);
    if (iterator == this->children.end()) {
        return nullptr;
    }
    return &(iterator->second);
}

////////////////////////////////////////////////////////////////////////////////
// sub_hierarchy
//
// An explicit version of sub_hierarchy that takes an initalizer list to
// prevent ambiguity between the vector and string overloads of the function.
////////////////////////////////////////////////////////////////////////////////
const StringHierarchy *StringHierarchy::sub_hierarchy(
    const std::initializer_list<std::string> input) const {
    std::vector<std::string> vec;
    vec.insert(vec.end(), input.begin(), input.end());
    return this->sub_hierarchy(vec);
}

////////////////////////////////////////////////////////////////////////////////
// sub_hierarchy
//
// Get a subtree of the StringHierarchy in order to avoid needing to query the
// entire hierarchy in the case where the use case is traversing down a tree
// anyways and does not want to keep track of the parent's values.
////////////////////////////////////////////////////////////////////////////////
const StringHierarchy *StringHierarchy::sub_hierarchy(
    const std::vector<std::string> &path) const {
    const StringHierarchy *sub_hierarchy = this;
    for (size_t i = 0; i < path.size(); i++) {
        sub_hierarchy = sub_hierarchy->sub_hierarchy(path[i]);
        // Escape before segfaulting.
        if (sub_hierarchy == nullptr) {
            return nullptr;
        }
    }
    return sub_hierarchy;
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
