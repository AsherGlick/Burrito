#pragma once

#include <string>
#include <vector>
using namespace std;

bool matches_any(string test, std::initializer_list<string> list);
bool normalized_matches_any(string test, std::initializer_list<string> list);
bool normalized_matches_any(string test, std::vector<string> list);

string lowercase(string);

vector<string> split(string input, string delimiter);

string normalize(string input_string);

std::string base64_encode(uint8_t const* buf, unsigned int bufLen);
std::vector<uint8_t> base64_decode(std::string const&);
