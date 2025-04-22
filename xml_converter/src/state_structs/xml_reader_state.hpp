#pragma once

#include <map>
#include <string>

class Category;

////////////////////////////////////////////////////////////////////////////////
// XMLReaderState
//
// A set of variables that are persisted while reading a taco file file.
// TODO: This should be renamed TacoReaderState
////////////////////////////////////////////////////////////////////////////////
struct XMLReaderState {
    // The root path of the marker pack for relative file paths.
    // TODO: This should be renamed to marker_pack_root_path because not all roots are directories
    std::string marker_pack_root_directory;
};
