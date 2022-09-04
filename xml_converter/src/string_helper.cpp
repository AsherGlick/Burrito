#include "string_helper.hpp"

#include <iostream>
#include <cstddef>
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

bool nomralized_matches_any(string test, std::initializer_list<string> list) {
    test = normalize(test);
    for (auto elem : list) {
        if (test == normalize(elem)) {
            return true;
        }
    }
    return false;
}

bool nomralized_matches_any(string test, std::vector<string> list) {
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
static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(uint8_t c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(uint8_t const* buf, unsigned int bufLen) {
  std::string ret;
  int i = 0;
  int j = 0;
  uint8_t char_array_3[3];
  uint8_t char_array_4[4];

  while (bufLen--) {
    char_array_3[i++] = *(buf++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++){
        ret += base64_chars[char_array_4[i]];
        }   
      i = 0;
    }
  }

  if (i){
    for(j = i; j < 3; j++){
      char_array_3[j] = '\0';
    }

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++){
      ret += base64_chars[char_array_4[j]];
    }

    while((i++ < 3)){
      ret += '=';
    }
  }

  return ret;
}

std::vector<uint8_t> base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  uint8_t char_array_4[4], char_array_3[3];
  std::vector<uint8_t> ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
        for (i = 0; i <4; i++){
        char_array_4[i] = base64_chars.find(char_array_4[i]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (i = 0; (i < 3); i++){
            ret.push_back(char_array_3[i]);
        }
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++){
        char_array_4[j] = 0;
    }

    for (j = 0; j <4; j++){
        char_array_4[j] = base64_chars.find(char_array_4[j]);
    }

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++){
        ret.push_back(char_array_3[j]);
    }
  }

  return ret;
}