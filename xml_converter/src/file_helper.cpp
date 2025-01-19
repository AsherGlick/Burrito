#include "file_helper.hpp"

#include <dirent.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "string_helper.hpp"

using namespace std;

// This function is a workaround that simulates the copy
// function in filesystem in C++17. Can be changed in future.
void copy_file(string path, string new_path) {
    ifstream infile(path, ios::binary);
    ofstream outfile(new_path, ios::binary);
    outfile << infile.rdbuf();
}

////////////////////////////////////////////////////////////////////////////////
// MarkerPackFile::MarkerPackFile
//
// Constructor for the MarkerPackFile class
////////////////////////////////////////////////////////////////////////////////
MarkerPackFile::MarkerPackFile(std::string base, std::string relative_filepath) {
    this->base = base;
    this->relative_filepath = relative_filepath;
}

// Temporary compatibility function mimicing the old string filepath
// functionality until it is removed.
const string MarkerPackFile::tmp_get_path() const {
    return join_file_paths(this->base, this->relative_filepath);
}

////////////////////////////////////////////////////////////////////////////////
// marker_pack_file_comp
//
// Helper function to compare the full file paths of marker pack files so that
// they can be sorted deterministically.
////////////////////////////////////////////////////////////////////////////////
bool marker_pack_file_comp(const MarkerPackFile& a, const MarkerPackFile& b) {
    if (a.base == b.base) {
        return lowercase(a.relative_filepath) < lowercase(b.relative_filepath);
    }
    return lowercase(a.base) < lowercase(b.base);
}

////////////////////////////////////////////////////////////////////////////////
// get_files_by_suffix
//
// Gets all the marker pack files with a given `suffix` inside of a marker pack
// filepath `base`
////////////////////////////////////////////////////////////////////////////////
vector<MarkerPackFile> get_files_by_suffix(
    const string& base,
    const string& suffix,
    const string& subpath) {
    vector<MarkerPackFile> files;

    if (!filesystem::exists(base)) {
        cout << "Error: " << base << " does not exist" << endl;
        return vector<MarkerPackFile>();
    }
    // If it is a directory, call open_directory_file_to_read
    else if (filesystem::is_directory(base)) {
        DIR* dir = opendir(join_file_paths(base, subpath).c_str());
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            string filename = entry->d_name;
            if (filename == ".") {
                continue;
            }
            else if (filename == "..") {
                continue;
            }

            if (entry->d_type == DT_DIR) {
                string new_subpath = join_file_paths(subpath, filename);
                vector<MarkerPackFile> subfiles = get_files_by_suffix(base, suffix, new_subpath);
                files.reserve(files.size() + subfiles.size());
                move(begin(subfiles), end(subfiles), back_inserter(files));
            }
            else if (has_suffix(filename, suffix)) {
                MarkerPackFile new_file(base, join_file_paths(subpath, filename));
                files.push_back(new_file);
            }
        }
        closedir(dir);
    }
    // else if (filesystem::is_regular_file(file.base)) {
    // }

    sort(files.begin(), files.end(), marker_pack_file_comp);
    return files;
}

vector<MarkerPackFile> get_files_by_suffix(const string& base, const string& suffix) {
    return get_files_by_suffix(base, suffix, "");
}
