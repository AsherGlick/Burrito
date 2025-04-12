#pragma once

#include <set>
#include <string>

#include "../rapidxml-1.13/rapidxml.hpp"

struct XMLWriterState {
    std::string marker_pack_root_directory;
    rapidxml::xml_document<char> *doc;
    std::set<std::string> written_textures;
};
