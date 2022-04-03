#include <cstddef>
#include <string>
#include <vector>
#include "string_helper.hpp"
using namespace std;

bool matches_any(string test, std::initializer_list<string> list) {
    for( auto elem : list ) {
        if (test == elem) {
            return true;
        }
    }
    return false;
}

bool nomralized_matches_any(string test, std::initializer_list<string> list) {
    test = normalize_type_name(test);
    for( auto elem : list ) {
        if (test == normalize_type_name(elem)) {
            return true;
        }
    }
    return false;
}

bool nomralized_matches_any(string test, std::vector<string> list) {
    test = normalize_type_name(test);
    for( auto elem : list ) {
        if (test == normalize_type_name(elem)) {
            return true;
        }
    }
    return false;
}


vector<string> split(string input, string delimiter) { 
    vector<string> output;
    size_t cursor_position = 0;
    while((cursor_position = input.find(delimiter)) != std::string::npos) {
        output.push_back(input.substr(0, cursor_position));
        input.erase(0, cursor_position + delimiter.length());
    }
    output.push_back(input);
    return output;
}


string normalize_type_name(string type_name) {
    string output;
    output.reserve(type_name.length());


    size_t i = 0;
    for (char character : type_name) {
        if (character >= 'A' && character <= 'Z') {
            output += (character - 'A' + 'a');
        }
        else if (character >= 'a' && character <= 'z') {
            output += character;
        }
    }

    return output;
}
