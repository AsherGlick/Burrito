#pragma once

#include <set>
#include <string>

#include "../rapidxml-1.13/rapidxml.hpp"

////////////////////////////////////////////////////////////////////////////////
// XMLWriterState
//
// A set of variables that are persisted while writing a taco file.
// TODO: This should be renamed to TacoWriterState
////////////////////////////////////////////////////////////////////////////////
struct XMLWriterState {
    // The root path of the marker pack for relative file paths.
    // TODO: This should be renamed to marker_pack_root_path because not all roots are directories
    std::string marker_pack_root_directory;

    // A reference to the rapidxml document that is being written to
    rapidxml::xml_document<char> *doc;

    // A set of all the textures that have been written to this taco file already
    std::set<std::string> written_textures;
};
