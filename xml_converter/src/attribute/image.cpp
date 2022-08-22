#include "image.hpp"

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

Image parse_image(rapidxml::xml_attribute<>* input, vector<XMLError*> *) {
    Image image;
    image.path = get_attribute_value(input);
    image.original_token = input;
    return image;
}
