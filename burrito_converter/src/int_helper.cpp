#include "int_helper.hpp"

#include <algorithm>

using namespace std;

bool is_string_valid_integer(const string test) {
    if (test.empty()) return false;
    return std::all_of(test.begin(), test.end(), ::isdigit);
}
