#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;
namespace waypoint {
class GUID;
}

class UniqueId {
 public:
    std::vector<uint8_t> guid;
};

void xml_attribute_to_unique_id(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    UniqueId* value,
    bool* is_set);

std::string unique_id_to_xml_attribute(const std::string& attribute_name, const UniqueId* value);

UniqueId from_proto_unique_id(std::string attribute_value);

void unique_id_to_proto(UniqueId value, std::function<void(std::string)> setter);
