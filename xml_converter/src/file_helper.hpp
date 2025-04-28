#pragma once
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class MarkerPackFile {
 public:
    MarkerPackFile(std::string base, std::string relative_filepath);

    const std::string tmp_get_path() const;

    std::string base;
    std::string relative_filepath;
};

void copy_file(MarkerPackFile original_path, MarkerPackFile new_path);

std::vector<MarkerPackFile> get_files_by_suffix(const std::string& base, const std::string& suffix);

std::unique_ptr<std::basic_istream<char>> read_file(const MarkerPackFile& file);

bool write_file(const MarkerPackFile& file, const std::stringstream& file_content);
