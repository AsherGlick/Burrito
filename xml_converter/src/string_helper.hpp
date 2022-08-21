#pragma once

#include <string>
#include <vector>
using namespace std;

bool matches_any(string test, std::initializer_list<string> list);
bool normalized_matches_any(string test, std::initializer_list<string> list);
bool normalized_matches_any(string test, std::vector<string> list);

string lowercase(string);

vector<string> split(string input, string delimiter);

string normalize_type_name(string type_name);

#ifndef _BASE64_H_
#define _BASE64_H_

typedef unsigned char BYTE;

std::string base64_encode(BYTE const* buf, unsigned int bufLen);
std::vector<BYTE> base64_decode(std::string const&);

#endif