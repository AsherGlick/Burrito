#pragma once

#include <string>
#include <vector>
using namespace std;

bool matches_any(string test, std::initializer_list<string> list);
bool nomralized_matches_any(string test, std::initializer_list<string> list);
bool nomralized_matches_any(string test, std::vector<string> list);

vector<string> split(string input, string delimiter);

string normalize_type_name(string type_name);
