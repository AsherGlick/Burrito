#include "file_helper.hpp"

#include <dirent.h>
#include <zip.h>

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
    // If it is a directory and parse the directory contents.
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
    // If it is a file, assume it is a zip file and read the zip contents.
    else if (filesystem::is_regular_file(base)) {
        int error = 0;
        zip* zip_archive = zip_open(base.c_str(), ZIP_RDONLY, &error);

        if (zip_archive == nullptr) {
            cerr << "Error: could not open the zip archive " << base << ". Got Error code " << error << endl;
            return files;
        }

        int64_t num_entries = zip_get_num_entries(zip_archive, 0x00);
        for (int64_t i = 0; i < num_entries; i++) {
            const char* filename = zip_get_name(zip_archive, i, 0x00);

            if (has_suffix(filename, suffix)) {
                MarkerPackFile new_file(base, filename);
                files.push_back(new_file);
            }
        }
    }
    else {
        cerr << "Error: " << base << " is not a file or directory" << endl;
    }

    sort(files.begin(), files.end(), marker_pack_file_comp);
    return files;
}

vector<MarkerPackFile> get_files_by_suffix(const string& base, const string& suffix) {
    return get_files_by_suffix(base, suffix, "");
}

////////////////////////////////////////////////////////////////////////////////
// _open_directory_file_for_read
//
// Helper function for `open_file_for_read()` to open a file that is inside of
// a directory.
////////////////////////////////////////////////////////////////////////////////
unique_ptr<basic_istream<char>> _open_directory_file_for_read(
    const string& base,
    const string& filename) {
    unique_ptr<ifstream> input_filestream = make_unique<ifstream>();
    input_filestream->open(join_file_paths(base, filename), ios::in | ios::binary);

    unique_ptr<basic_istream<char>> basic_istream_stream(move(input_filestream));

    return basic_istream_stream;
}

////////////////////////////////////////////////////////////////////////////////
// _open_zip_file_for_read
//
// Helper function for `open_file_for_read()` to open a file that is inside of
// a zipfile.
////////////////////////////////////////////////////////////////////////////////
unique_ptr<basic_istream<char>> _open_zip_file_for_read(
    const string& zipfile,
    const string& filename) {
    int error = 0;
    struct zip* zip_archive = zip_open(zipfile.c_str(), ZIP_RDONLY, &error);

    if (zip_archive == nullptr) {
        cerr << "Error: could not open the zip archive " << zipfile << ". Got Error code " << error << endl;
        return nullptr;
    }

    // Search for the filenname
    const char* name = filename.c_str();
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(zip_archive, name, 0x00, &st);

    char* contents = new char[st.size];
    zip_file_t* f = zip_fopen(zip_archive, name, 0x00);

    // Early exit if the file cannot be opened
    if (f == nullptr) {
        return nullptr;
    }

    zip_fread(f, contents, st.size);
    zip_fclose(f);
    zip_close(zip_archive);

    string sized_contents(contents, st.size);

    // Copy the file string into the stringstring and move it into a basic_istream
    unique_ptr<istringstream> string_stream = make_unique<istringstream>(sized_contents, ios_base::in | ios_base::binary);
    unique_ptr<basic_istream<char>> basic_istream_stream(move(string_stream));
    delete[] contents;

    return basic_istream_stream;
}

////////////////////////////////////////////////////////////////////////////////
// open_file_for_read
//
// Opens a file to read from that is either inside of a directory or inside of
// a zip file. If `base` is a file then it will be assumed to be a zip file and
// `filename` will be opened inside of it. If `base` is a directory then
// `filename` inside of that directory will be opened instead.
////////////////////////////////////////////////////////////////////////////////
unique_ptr<basic_istream<char>> open_file_for_read(const MarkerPackFile& file) {
    if (!filesystem::exists(file.base)) {
        cout << "Error: " << file.base << " does not exist" << endl;
        return nullptr;
    }
    // If it is a directory, call open_directory_file_to_read
    else if (filesystem::is_directory(file.base)) {
        return _open_directory_file_for_read(file.base, file.relative_filepath);
    }
    // If it is a file call open_zip_file_to_read
    else if (filesystem::is_regular_file(file.base)) {
        return _open_zip_file_for_read(file.base, file.relative_filepath);
    }

    cout << "Error: " << file.base << " is not a directory or a file" << endl;
    return nullptr;
}
