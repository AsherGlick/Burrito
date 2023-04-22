#include "file_helper.hpp"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void copy_file(string path, string new_path) {
    ifstream infile(path, ios::binary);
    ofstream outfile(new_path, ios::binary);
    outfile << infile.rdbuf();
}
