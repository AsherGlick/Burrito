#include "file_helper.hpp"

#include <dirent.h>

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "miniz-3.0.2/miniz.hpp"
#include "string_helper.hpp"

using namespace std;

void copy_file(MarkerPackFile original_path, MarkerPackFile new_path) {
    if (original_path.tmp_get_path() == new_path.tmp_get_path()) {
        cerr << "File is being copied to itself" << original_path.tmp_get_path() << endl;
        return;
    }

    // Directory to Directory
    if (filesystem::is_directory(original_path.base) && filesystem::is_directory(new_path.base)) {
        if (!filesystem::exists(filesystem::path(original_path.tmp_get_path()))) {
            cerr << "File does not exist to copy from " << original_path.tmp_get_path() << endl;
            return;
        }
        filesystem::path output_path = filesystem::path(new_path.tmp_get_path());
        filesystem::create_directories(output_path.parent_path());
        filesystem::copy_file(
            filesystem::path(original_path.tmp_get_path()),
            output_path,
            filesystem::copy_options::overwrite_existing
        );
    }
    else {
        auto original_file = read_file(original_path);

        if (original_file == nullptr) {
            cerr << "File does not exist to copy from " << original_path.tmp_get_path() << endl;
            return;
        }

        // TODO: This seems like we are copying the data twice in memory.
        // Probably some element of this can be optimized to not do that.
        stringstream data;
        data << original_file->rdbuf();

        write_file(new_path, data);
    }
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
    const string& subpath
) {
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
        mz_zip_archive zip_archive;
        memset(&zip_archive, 0, sizeof(zip_archive));

        mz_bool status = mz_zip_reader_init_file(&zip_archive, base.c_str(), 0);
        if (!status) {
            cerr << "Error: could not open the zip archive " << base << ". Got Error code " << status << endl;
            return files;
        }

        for (unsigned int i = 0; i < mz_zip_reader_get_num_files(&zip_archive); i++) {
            mz_zip_archive_file_stat file_stat;

            if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
                cerr << "Error: could not get file stat info in " << base << " for the file at index " << i << endl;
                continue;
            }

            if (has_suffix(file_stat.m_filename, suffix)) {
                MarkerPackFile new_file(base, file_stat.m_filename);
                files.push_back(new_file);
            }
        }

        mz_zip_reader_end(&zip_archive);
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
static unique_ptr<basic_istream<char>> _open_directory_file_for_read(
    const string& base,
    const string& filename
) {
    unique_ptr<ifstream> input_filestream = make_unique<ifstream>();

    string filepath = join_file_paths(base, filename);
    if (!filesystem::exists(filepath)) {
        return nullptr;
    }

    input_filestream->open(filepath, ios::in | ios::binary);

    unique_ptr<basic_istream<char>> basic_istream_stream(move(input_filestream));

    return basic_istream_stream;
}

////////////////////////////////////////////////////////////////////////////////
// _open_zip_file_for_read
//
// Helper function for `open_file_for_read()` to open a file that is inside of
// a zipfile.
////////////////////////////////////////////////////////////////////////////////
static unique_ptr<basic_istream<char>> _open_zip_file_for_read(
    const string& zipfile,
    const string& filename
) {
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    mz_bool status = mz_zip_reader_init_file(&zip_archive, zipfile.c_str(), 0);

    if (!status) {
        cerr << "Error: could not open the zip archive " << zipfile << ". Got Error code " << status << endl;
        return nullptr;
    }

    size_t uncomp_size;
    void* uncomp_data = mz_zip_reader_extract_file_to_heap(&zip_archive, filename.c_str(), &uncomp_size, 0);

    if (uncomp_data == nullptr) {
        mz_zip_reader_end(&zip_archive);
        return nullptr;
    }

    // Copy the file string into the stringstring and move it into a basic_istream
    string sized_contents(reinterpret_cast<char*>(uncomp_data), uncomp_size);
    unique_ptr<istringstream> string_stream = make_unique<istringstream>(sized_contents, ios_base::in | ios_base::binary);
    unique_ptr<basic_istream<char>> basic_istream_stream(move(string_stream));

    mz_free(uncomp_data);
    mz_zip_reader_end(&zip_archive);

    return basic_istream_stream;
}

////////////////////////////////////////////////////////////////////////////////
// read_file
//
// Opens a file to read from that is either inside of a directory or inside of
// a zip file. If `base` is a file then it will be assumed to be a zip file and
// `filename` will be opened inside of it. If `base` is a directory then
// `filename` inside of that directory will be opened instead.
////////////////////////////////////////////////////////////////////////////////
unique_ptr<basic_istream<char>> read_file(const MarkerPackFile& file) {
    // cout << "Open File for Read" << endl;
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

static bool _write_directory_file(
    const string& base,
    const string& filename,
    const stringstream& file_content
) {
    string filepath = join_file_paths(base, filename);

    filesystem::path output_path = filesystem::path(filepath);
    filesystem::create_directories(output_path.parent_path());

    ofstream output_file(filepath);

    output_file << file_content.rdbuf();
    output_file.close();
    return true;
}

static bool _write_zip_file(
    const string& zipfile,
    const string& filename,
    const stringstream& file_content
) {
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    // Start a new archive
    if (!mz_zip_writer_init_file(&zip_archive, zipfile.c_str(), 0)) {
        std::cerr << "Failed to initialize zip file: " << zipfile << endl;
        return false;
    }

    auto file_buffer = file_content.str();

    // Add the memory buffer as a file into the zip
    if (!mz_zip_writer_add_mem(&zip_archive, filename.c_str(), file_buffer.c_str(), file_buffer.size(), MZ_BEST_COMPRESSION)) {
        std::cerr << "Failed to add file to zip." << std::endl;
        mz_zip_writer_end(&zip_archive);
        return false;
    }

    // Finalize and close the archive
    if (!mz_zip_writer_finalize_archive(&zip_archive)) {
        std::cerr << "Failed to finalize zip archive." << std::endl;
        mz_zip_writer_end(&zip_archive);
        return false;
    }

    // Cleanup
    mz_zip_writer_end(&zip_archive);
    return true;
}

bool write_file(const MarkerPackFile& file, const stringstream& file_content) {
    if (filesystem::exists(file.base) && filesystem::is_regular_file(file.base)) {
        return _write_zip_file(file.base, file.relative_filepath, file_content);
    }
    else {
        return _write_directory_file(file.base, file.relative_filepath, file_content);
    }
}
