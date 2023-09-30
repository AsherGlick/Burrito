#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

int parse_int(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string stringify_int(int attribute_value);

// Zero Cost Abstraction identity functions to make parsing and writing protobufs more uniform
inline int const& from_proto_int(const int &x) {return x;}
inline int const& to_proto_int(const int &x) {return x;}
