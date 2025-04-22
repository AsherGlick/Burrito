#include "string_helper.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

bool matches_any(string test, std::initializer_list<string> list) {
    for (auto elem : list) {
        if (test == elem) {
            return true;
        }
    }
    return false;
}

bool normalized_matches_any(string test, std::initializer_list<string> list) {
    test = normalize(test);
    for (auto elem : list) {
        if (test == normalize(elem)) {
            return true;
        }
    }
    return false;
}

bool normalized_matches_any(string test, std::vector<string> list) {
    test = normalize(test);
    for (auto elem : list) {
        if (test == normalize(elem)) {
            return true;
        }
    }
    return false;
}

vector<string> split(string input, string delimiter) {
    vector<string> output;
    size_t cursor_position = 0;
    while ((cursor_position = input.find(delimiter)) != std::string::npos) {
        output.push_back(input.substr(0, cursor_position));
        input.erase(0, cursor_position + delimiter.length());
    }
    output.push_back(input);
    return output;
}

string join(const vector<string>& input, const string& delimiter) {
    string result;
    size_t size = 0;
    for (size_t i = 0; i < input.size(); i++) {
        size += input[i].size() + delimiter.size();
    }

    result.reserve(size);

    for (size_t i = 0; i < input.size(); i++) {
        result += input[i];
        // Don't add delimiter after the last element
        if (i < input.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// normalize
//
// A speedy function to return a normalized copy of a string. Normalization
// happens according to the lookup table defined for this function.
//
// This lookup table maps:
//   All Numbers 0-9 to themselves
//   All Lowercase Letters a-z to themselves
//   All Uppercase Letters A-Z to the lowercase letters a-z
//   Everything else to 0
////////////////////////////////////////////////////////////////////////////////
// clang-format off
static unsigned char normalize_lookup[256] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,   0,   0,
      0,   0,   0,   0,   0,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106,
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
    122,   0,   0,   0,   0,   0,   0,  97,  98,  99, 100, 101, 102, 103, 104,
    105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0
};
// clang-format on
string normalize(string input_string) {
    size_t out_index = 0;
    for (size_t i = 0; i < input_string.length(); i++) {
        unsigned char new_char = normalize_lookup[(unsigned char)input_string[i]];
        input_string[out_index] = new_char;
        out_index += (new_char > 0);
    }

    input_string.erase(out_index);
    return input_string;
}

////////////////////////////////////////////////////////////////////////////////
// lowercase
//
// A function to return a string where all of the letters have been lowercased.
// Only works on ASCII strings.
////////////////////////////////////////////////////////////////////////////////
string lowercase(string input) {
    string output;
    output.reserve(input.length());

    for (char character : input) {
        if (character >= 'A' && character <= 'Z') {
            output += (character - 'A' + 'a');
        }
        else {
            output += character;
        }
    }
    return output;
}

// Functions to either encode or decode base64 strings

// Obtained from https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string base64_encode(uint8_t const* buf, unsigned int bufLen) {
    std::string ret;
    int input_chunk_index = 0;
    int output_chunk_index = 0;
    uint8_t input_chunk[3];
    uint8_t output_chunk[4];

    while (bufLen--) {
        input_chunk[input_chunk_index++] = *(buf++);
        if (input_chunk_index == 3) {
            output_chunk[0] = (input_chunk[0] & 0xfc) >> 2;
            output_chunk[1] = ((input_chunk[0] & 0x03) << 4) + ((input_chunk[1] & 0xf0) >> 4);
            output_chunk[2] = ((input_chunk[1] & 0x0f) << 2) + ((input_chunk[2] & 0xc0) >> 6);
            output_chunk[3] = input_chunk[2] & 0x3f;

            for (output_chunk_index = 0; output_chunk_index < 4; output_chunk_index++) {
                ret += base64_chars[output_chunk[output_chunk_index]];
            }
            input_chunk_index = 0;
        }
    }

    if (input_chunk_index > 0) {
        for (output_chunk_index = input_chunk_index; output_chunk_index < 3; output_chunk_index++) {
            input_chunk[output_chunk_index] = 0;
        }

        output_chunk[0] = (input_chunk[0] & 0xfc) >> 2;
        output_chunk[1] = ((input_chunk[0] & 0x03) << 4) + ((input_chunk[1] & 0xf0) >> 4);
        output_chunk[2] = ((input_chunk[1] & 0x0f) << 2) + ((input_chunk[2] & 0xc0) >> 6);
        output_chunk[3] = input_chunk[2] & 0x3f;

        for (output_chunk_index = 0; (output_chunk_index < input_chunk_index + 1); output_chunk_index++) {
            ret += base64_chars[output_chunk[output_chunk_index]];
        }

        while (input_chunk_index++ < 3) {
            ret += '=';
        }
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
// This lookup table maps all base64 characters to their numerical equivalents
//   A-Z  0-25
//   a-z 26-51
//   0-9 52-61
//   +   62
//   /   63
//   Everything else to 255 (invalid character)
//   = is invalid because trailing ='s are stripped any any others are invalid
////////////////////////////////////////////////////////////////////////////////
// clang-format off
static unsigned char base64_lookup[256] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  62, 255,
    255, 255,  63,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
    255, 255, 255, 255, 255,  0,    1,   2,   3,   4,   5,   6,   7,   8,   9,
     10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
     25, 255, 255, 255, 255, 255, 255,  26,  27,  28,  29,  30,  31,  32,  33,
     34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
     49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255
};
// clang-format on

std::vector<uint8_t> base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();

    uint8_t char_array_4[4];

    size_t input_index = 0;
    size_t output_index = 0;

    while (encoded_string[in_len - 1] == '=') {
        in_len -= 1;
    }

    std::vector<uint8_t> ret(in_len * 3 / 4);

    while (in_len >= 4) {
        for (int i = 0; i < 4; i++) {
            char_array_4[i] = base64_lookup[(uint8_t)encoded_string[input_index + i]];

            if (char_array_4[i] == 255) {
                // TODO: Throw an error or something
                std::cerr << "Found an invalid letter when decoding base64" << std::endl;
                return std::vector<uint8_t>();
            }
        }

        ret[output_index] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        ret[output_index + 1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        ret[output_index + 2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        input_index += 4;
        in_len -= 4;
        output_index += 3;
    }

    if (in_len) {
        int i = 0;
        for (; i < in_len; i++) {
            char_array_4[i] = base64_lookup[(uint8_t)encoded_string[input_index + i]];

            if (char_array_4[i] == 255) {
                // TODO: Throw an error or something
                std::cerr << "Found an invalid letter when decoding base64" << std::endl;
                return std::vector<uint8_t>();
            }
        }
        for (; i < 4; i++) {
            char_array_4[i] = 0;
        }

        ret[output_index] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        ret[output_index + 1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        ret[output_index + 2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
    }

    return ret;
}

string get_base_dir(string filepath) {
    size_t s = filepath.find_last_of("/");
    return filepath.substr(0, s);
}

bool has_suffix(std::string const& fullString, std::string const& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}

string join_file_paths(const string& path_a, const string& path_b) {
    if (path_a.empty()) {
        return path_b;
    }
    if (path_b.empty()) {
        return path_a;
    }

    string output = path_a;
    if (!has_suffix(path_a, "/")) {
        output += "/";
    }
    return output + path_b;
}

////////////////////////////////////////////////////////////////////////////////
// long_to_hex_string
//
// A helper function that converts an 8 byte long into a 16 byte hex string.
////////////////////////////////////////////////////////////////////////////////
static const char* hex_chars = "0123456789abcdef";
std::string long_to_hex_string(uint64_t number) {
    std::string hex_string(16, '0');

    for (int i = 15; i >= 0; --i) {
        hex_string[i] = hex_chars[number & 0xF];
        number >>= 4;
    }

    return hex_string;
}

// clang-format off
static const unsigned char hex_lookup[256] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9, 255, 255,
    255, 255, 255, 255, 255,  10,  11,  12,  13,  14,  15, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255,  10,  11,  12,  13,  14,  15, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255
};
// clang-format on

bool is_hex(const string& value) {
    for (size_t i = 0; i < value.length(); i++) {
        if (hex_lookup[(uint8_t)value[i]] == 255) {
            return false;
        }
    }
    return true;
}
