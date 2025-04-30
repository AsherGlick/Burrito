#pragma once

#include <cstdint>
#include <initializer_list>
#include <set>
#include <string>
#include <vector>

bool matches_any(std::string test, std::initializer_list<std::string> list);
bool normalized_matches_any(std::string test, std::initializer_list<std::string> list);
bool normalized_matches_any(std::string test, std::vector<std::string> list);

std::string lowercase(std::string);

std::vector<std::string> split(std::string input, std::string delimiter);
std::string join(const std::vector<std::string>& input, const std::string& delimiter);

std::string normalize(std::string input_string);

std::string base64_encode(uint8_t const* buf, unsigned int bufLen);
std::vector<uint8_t> base64_decode(std::string const&);

std::string get_base_dir(std::string filepath);
bool has_suffix(std::string const& fullString, std::string const& ending);
std::string join_file_paths(const std::string& path_a, const std::string& path_b);

std::string long_to_hex_string(uint64_t number);

bool is_hex(const std::string& value);
