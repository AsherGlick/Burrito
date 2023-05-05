#include "file_helper.hpp"

#include <fstream>
#include <string>

using namespace std;

// This function is a workaround that simulates the copy
// function in filesystem in C++17. Can be changed in future.
void copy_file(string path, string new_path) {
    ifstream infile(path, ios::binary);
    ofstream outfile(new_path, ios::binary);
    outfile << infile.rdbuf();
}
