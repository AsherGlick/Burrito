#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;
class UniqueId {
 public:
    vector<uint8_t> guid;
};

UniqueId parse_unique_id(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);
