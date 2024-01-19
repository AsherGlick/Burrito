#pragma once

#include <string>
#include <vector>

class Image;

struct XMLWriterState {
    const char* xml_filedir;

    // A list of all of the textures with their paths.
    std::vector<const Image*> textures;
};
