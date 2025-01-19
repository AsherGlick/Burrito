#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#include "attribute/marker_category.hpp"
#include "category_gen.hpp"
#include "file_helper.hpp"
#include "guildpoint.pb.h"
#include "icon_gen.hpp"
#include "packaging_protobin.hpp"
#include "packaging_xml.hpp"
#include "parseable.hpp"
#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "string_helper.hpp"
#include "trail_gen.hpp"

using namespace std;

bool filename_comp(string a, string b) {
    return lowercase(a) < lowercase(b);
}

struct PathConfig {
    string type;  // "import" or "export" or "template"
    string format;  // "xml" or "guildpoint"
    vector<string> paths;
    bool allow_duplicates = false;
    // bool split_by_category = false;
    bool split_by_map_id = false;

    PathConfig(const string& type, const string& format)
        : type(type), format(format) {
    }
};

// Searchs for files within a directory with a suffix and returns their relative paths.
vector<string> get_files_by_suffix(string directory, string suffix) {
    vector<string> files;
    DIR* dir = opendir(directory.c_str());
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        string filename = entry->d_name;
        if (filename != "." && filename != "..") {
            string path = join_file_paths(directory, filename);
            if (entry->d_type == DT_DIR) {
                vector<string> subfiles = get_files_by_suffix(path, suffix);
                // Default: markerpacks have all xml files in the first directory
                for (string subfile : subfiles) {
                    cout << subfile << " found in subfolder" << endl;
                    files.push_back(join_file_paths(filename, subfile));
                }
            }
            else if (has_suffix(filename, suffix)) {
                files.push_back(filename);
            }
        }
    }
    closedir(dir);
    std::sort(files.begin(), files.end(), filename_comp);
    return files;
}

map<string, vector<string>> read_taco_directory(
    string input_path,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois) {
    map<string, vector<string>> top_level_category_file_locations;
    if (!filesystem::exists(input_path)) {
        cout << "Error: " << input_path << " is not an existing directory or file" << endl;
    }
    else if (filesystem::is_directory(input_path)) {
        string directory_name = filesystem::path(input_path).filename();
        vector<string> xml_files = get_files_by_suffix(input_path, ".xml");
        for (const string& path : xml_files) {
            set<string> top_level_category_names = parse_xml_file(join_file_paths(input_path, path), input_path, marker_categories, parsed_pois);
            string relative_path = join_file_paths(directory_name, path);
            for (set<string>::iterator it = top_level_category_names.begin(); it != top_level_category_names.end(); it++) {
                top_level_category_file_locations[*it].push_back(relative_path);
            }
        }
    }
    else if (filesystem::is_regular_file(input_path)) {
        set<string> top_level_category_names = parse_xml_file(input_path, get_base_dir(input_path), marker_categories, parsed_pois);
        string filename = filesystem::path(input_path).filename();
        for (set<string>::iterator it = top_level_category_names.begin(); it != top_level_category_names.end(); it++) {
            top_level_category_file_locations[*it].push_back(filename);
        }
    }
    return top_level_category_file_locations;
}

map<string, vector<string>> read_burrito_directory(
    string input_path,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois) {
    map<string, vector<string>> top_level_category_file_locations;
    if (!filesystem::exists(input_path)) {
        cout << "Error: " << input_path << " is not an existing directory or file" << endl;
    }
    else if (filesystem::is_directory(input_path)) {
        string directory_name = filesystem::path(input_path).filename();
        vector<string> burrito_files = get_files_by_suffix(input_path, ".guildpoint");
        for (const string& path : burrito_files) {
            set<string> top_level_category_names = read_protobuf_file(join_file_paths(input_path, path), input_path, marker_categories, parsed_pois);
            string relative_path = join_file_paths(directory_name, path);
            for (set<string>::iterator it = top_level_category_names.begin(); it != top_level_category_names.end(); it++) {
                top_level_category_file_locations[*it].push_back(relative_path);
            }
        }
    }
    else if (filesystem::is_regular_file(input_path)) {
        set<string> top_level_category_names = read_protobuf_file(input_path, get_base_dir(input_path), marker_categories, parsed_pois);
        string filename = filesystem::path(input_path).filename();
        for (set<string>::iterator it = top_level_category_names.begin(); it != top_level_category_names.end(); it++) {
            top_level_category_file_locations[*it].push_back(filename);
        }
    }
    return top_level_category_file_locations;
}

void write_taco_directory(
    string output_path,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois) {
    // TODO: Exportion of XML Marker Packs File Structure #111
    if (!filesystem::is_directory(output_path)) {
        if (!filesystem::create_directory(output_path)) {
            cout << "Error: " << output_path << "is not a valid directory path" << endl;
            return;
        }
    }
    write_xml_file(output_path, marker_categories, parsed_pois);
}

void write_burrito_directory(
    PathConfig path_config,
    string output_path,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois) {
    if (!filesystem::is_directory(output_path)) {
        if (!filesystem::create_directory(output_path)) {
            cout << "Error: " << output_path << "is not a valid directory path" << endl;
            return;
        }
    }
    StringHierarchy category_filter;
    category_filter.add_path({}, true);
    if (path_config.split_by_map_id) {
        write_protobuf_file_per_map_id(output_path, category_filter, marker_categories, parsed_pois);
    }
    else {
        write_protobuf_file(output_path, category_filter, marker_categories, parsed_pois);
    }
}

////////////////////////////////////////////////////////////////////////////////
// process_data
//
// The universal entrypoint into the xml converter functionality. Both the CLI
// and the library entrypoints direct here to do their actual processing.
////////////////////////////////////////////////////////////////////////////////
void process_data(vector<PathConfig> path_configs) {
    // All of the loaded pois and categories
    vector<Parseable*> parsed_pois;
    map<string, Category> marker_categories;
    map<string, vector<vector<string>>> top_level_category_file_locations_by_pack;
    map<string, set<string>> duplicate_categories;

    // Read in all the xml taco markerpacks
    auto begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < path_configs.size(); i++) {
        if (path_configs[i].type != "import" || path_configs[i].format != "xml") {
            continue;
        }
        for (size_t j = 0; j < path_configs[i].paths.size(); j++) {
            cout << "Loading taco pack " << path_configs[i].paths[j] << endl;

            map<string, vector<string>> top_level_category_file_locations = read_taco_directory(
                path_configs[i].paths[j],
                &marker_categories,
                &parsed_pois);
            for (map<string, vector<string>>::iterator it = top_level_category_file_locations.begin(); it != top_level_category_file_locations.end(); it++) {
                top_level_category_file_locations_by_pack[it->first].push_back(it->second);
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The taco parse function took " << ms << " milliseconds to run" << endl;

    // Read in all the protobin guildpoint markerpacks
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < path_configs.size(); i++) {
        if (path_configs[i].type != "import" || path_configs[i].format != "guildpoint") {
            continue;
        }
        for (size_t j = 0; j < path_configs[i].paths.size(); j++) {
            cout << "Loading guildpoint pack " << path_configs[i].paths[j] << endl;

            map<string, vector<string>> top_level_category_file_locations = read_burrito_directory(
                path_configs[i].paths[j],
                &marker_categories,
                &parsed_pois);
            for (map<string, vector<string>>::iterator it = top_level_category_file_locations.begin(); it != top_level_category_file_locations.end(); it++) {
                top_level_category_file_locations_by_pack[it->first].push_back(it->second);
            }
        }
    }
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The guildpoint parse function took " << ms << " milliseconds to run" << endl;

    for (map<string, vector<vector<string>>>::iterator it = top_level_category_file_locations_by_pack.begin(); it != top_level_category_file_locations_by_pack.end(); it++) {
        if (it->second.size() != 1) {
            for (size_t i = 0; i < it->second.size(); i++) {
                for (size_t j = 0; j < it->second[i].size(); j++) {
                    duplicate_categories[it->first].insert(it->second[i][j]);
                }
            }
        }
    }

    bool all_allow_duplicates_false = all_of(path_configs.begin(), path_configs.end(), [](const PathConfig p) {
        if (p.type == "export") {
            return p.allow_duplicates == false;
        }
        return true;
    });

    if (duplicate_categories.size() > 0 && all_allow_duplicates_false == true) {
        cout << "Did not write due to duplicates in categories." << endl;
        cout << "This commonly occurs when attempting to read the same pack multiple times or when separate packs coincidentally have the same name." << endl;
        // TODO: This is the current advice. Further updates could allow other
        // options like selective merges or changing category names to be unique
        cout << "Please remove one of the packs or edit the name of the packs' top level category before running the program again." << endl;
        cout << "If you want to bypass this stop, use '--allow-duplicates'." << endl;
        cout << "The following top level categories were found in more than one pack:" << endl;
        for (map<string, set<string>>::iterator it = duplicate_categories.begin(); it != duplicate_categories.end(); it++) {
            cout << "    \"" << it->first << "\" in files:" << endl;
            for (string str : it->second) {
                cout << "        " << str << endl;
            }
        }
        return;
    }

    // Write all of the xml taco paths
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < path_configs.size(); i++) {
        if (path_configs[i].type != "export" || path_configs[i].format != "xml") {
            continue;
        }
        for (size_t j = 0; j < path_configs[i].paths.size(); j++) {
            write_taco_directory(path_configs[i].paths[j], &marker_categories, &parsed_pois);
        }
    }
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The xml write function took " << ms << " milliseconds to run" << endl;

    // Write all of the protobin guildpoint paths
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < path_configs.size(); i++) {
        if (path_configs[i].type != "export" || path_configs[i].format != "guildpoint") {
            continue;
        }
        for (size_t j = 0; j < path_configs[i].paths.size(); j++) {
            write_burrito_directory(path_configs[i], path_configs[i].paths[j], &marker_categories, &parsed_pois);
        }
    }
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The protobuf write function took " << ms << " milliseconds to run" << endl;
}

////////////////////////////////////////////////////////////////////////////////
// parse_arguments
// Processes all of the command line data into a format the internal functions
// want to receive.
//
// Example usage
//   ./xml_converter --input-taco-paths ../packs/marker_pack --output-guildpoint-path ../output_packs
//   ./xml_converter --input-taco-paths ../packs/* --output-guildpoint-path ../output_packs --split_by_map_id
//
// --import_FORMAT_paths <path> --FLAGS [--import_FORMAT_paths <path> --FLAGS]
// --export_FORMAT_paths <path> --filter "conditions" --FLAGS
// [--export_FORMAT_paths <path> --filter "conditions" --FLAGS]
////////////////////////////////////////////////////////////////////////////////
vector<PathConfig> parse_arguments(int argc, char* argv[]) {
    vector<PathConfig> path_configs;
    PathConfig* arg_target = nullptr;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--input-taco-path")) {
            path_configs.emplace_back("import", "xml");
            arg_target = &path_configs.back();
        }
        else if (!strcmp(argv[i], "--output-taco-path")) {
            path_configs.emplace_back("export", "xml");
            arg_target = &path_configs.back();
        }
        else if (!strcmp(argv[i], "--input-guildpoint-path")) {
            path_configs.emplace_back("import", "guildpoint");
            arg_target = &path_configs.back();
        }
        else if (!strcmp(argv[i], "--output-guildpoint-path")) {
            path_configs.emplace_back("export", "guildpoint");
            arg_target = &path_configs.back();
        }
        else if (arg_target != nullptr) {
            if (!strcmp(argv[i], "--allow-duplicates")) {
                arg_target->allow_duplicates = true;
            }
            else if (!strcmp(argv[i], "--split-by-map-id")) {
                arg_target->split_by_map_id = true;
            }
            // else if (!strcmp(argv[i], "--split-by-category")) {
            //     arg_target.split_by_category = true
            // }
            else {
                arg_target->paths.push_back(argv[i]);
            }
        }
        else {
            cout << "Unknown argument " << argv[i] << endl;
            return {};
        }
    }
    return path_configs;
}

////////////////////////////////////////////////////////////////////////////////
// main
//
// Main is the CLI entrypoint to the xml converter.
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    vector<PathConfig> parsed_arguments = parse_arguments(argc, argv);
    if (parsed_arguments.empty()) {
        return -1;
    }
    process_data(parsed_arguments);

    return 0;
}
