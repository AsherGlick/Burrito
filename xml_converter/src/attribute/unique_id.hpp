#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

class UniqueId {
 public:
    std::vector<uint8_t> guid;
};

UniqueId parse_unique_id(rapidxml::xml_attribute<>* input, std::vector<XMLError*> *errors);

std::string stringify_unique_id(UniqueId attribute_value);
