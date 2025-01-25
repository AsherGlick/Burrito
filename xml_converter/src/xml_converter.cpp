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
#include "argument_parser.hpp"
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

map<string, vector<string>> read_taco_directory(
    PathConfig path_config,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois) {
    map<string, vector<string>> top_level_category_file_locations;
    if (!filesystem::exists(path_config.path)) {
        cout << "Error: " << path_config.path << " is not an existing directory or file" << endl;
    }
    else if (filesystem::is_directory(path_config.path)) {
        string directory_name = filesystem::path(path_config.path).filename();
        vector<MarkerPackFile> xml_files = get_files_by_suffix(path_config.path, ".xml");
        for (const MarkerPackFile& path : xml_files) {
            set<string> top_level_category_names = parse_xml_file(path.tmp_get_path(), path_config.path, marker_categories, parsed_pois);
            string relative_path = join_file_paths(directory_name, path.relative_filepath);
            for (set<string>::iterator it = top_level_category_names.begin(); it != top_level_category_names.end(); it++) {
                top_level_category_file_locations[*it].push_back(relative_path);
            }
        }
    }
    return top_level_category_file_locations;
}

map<string, vector<string>> read_burrito_directory(
    PathConfig path_config,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois) {
    map<string, vector<string>> top_level_category_file_locations;
    if (!filesystem::exists(path_config.path)) {
        cout << "Error: " << path_config.path << " is not an existing directory or file" << endl;
    }
    else if (filesystem::is_directory(path_config.path)) {
        string directory_name = filesystem::path(path_config.path).filename();
        vector<MarkerPackFile> burrito_files = get_files_by_suffix(path_config.path, ".guildpoint");
        for (const MarkerPackFile& path : burrito_files) {
            set<string> top_level_category_names = read_protobuf_file(path.tmp_get_path(), path_config.path, marker_categories, parsed_pois);
            string relative_path = join_file_paths(directory_name, path.relative_filepath);
            for (set<string>::iterator it = top_level_category_names.begin(); it != top_level_category_names.end(); it++) {
                top_level_category_file_locations[*it].push_back(relative_path);
            }
        }
    }
    return top_level_category_file_locations;
}

void write_taco_directory(
    PathConfig path_config,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois) {
    // TODO: Exportion of XML Marker Packs File Structure #111
    if (!filesystem::is_directory(path_config.path)) {
        if (!filesystem::create_directory(path_config.path)) {
            cout << "Error: " << path_config.path << "is not a valid directory path" << endl;
            return;
        }
    }
    write_xml_file(path_config.path, marker_categories, parsed_pois);
}

void write_burrito_directory(
    PathConfig path_config,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois) {
    if (!filesystem::is_directory(path_config.path)) {
        if (!filesystem::create_directory(path_config.path)) {
            cout << "Error: " << path_config.path << "is not a valid directory path" << endl;
            return;
        }
    }
    StringHierarchy category_filter;
    category_filter.add_path({}, true);
    if (path_config.split_by_map_id) {
        write_protobuf_file_per_map_id(path_config.path, category_filter, marker_categories, parsed_pois);
    }
    else {
        write_protobuf_file(path_config.path, category_filter, marker_categories, parsed_pois);
    }
}

////////////////////////////////////////////////////////////////////////////////
// process_data
//
// The universal entrypoint into the xml converter functionality. Both the CLI
// and the library entrypoints direct here to do their actual processing.
////////////////////////////////////////////////////////////////////////////////
void process_data(ParsedArguments parsed_arguments) {
    // All of the loaded pois and categories
    vector<Parseable*> parsed_pois;
    map<string, Category> marker_categories;
    map<string, vector<vector<string>>> top_level_category_file_locations_by_pack;
    map<string, set<string>> duplicate_categories;
    vector<PathConfig> path_configs = parsed_arguments.path_configs;

    // Read in all the xml taco markerpacks
    auto begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < path_configs.size(); i++) {
        if (path_configs[i].type != BehaviorType::IMPORT || path_configs[i].format != MarkerFormat::XML) {
            continue;
        }
        cout << "Loading taco pack " << path_configs[i].path << endl;

        map<string, vector<string>> top_level_category_file_locations = read_taco_directory(
            path_configs[i],
            &marker_categories,
            &parsed_pois);
        for (map<string, vector<string>>::iterator it = top_level_category_file_locations.begin(); it != top_level_category_file_locations.end(); it++) {
            top_level_category_file_locations_by_pack[it->first].push_back(it->second);
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The taco parse function took " << ms << " milliseconds to run" << endl;

    // Read in all the protobin guildpoint markerpacks
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < path_configs.size(); i++) {
        if (path_configs[i].type != BehaviorType::IMPORT || path_configs[i].format != MarkerFormat::GUILDPOINT) {
            continue;
        }
        cout << "Loading guildpoint pack " << path_configs[i].path << endl;

        map<string, vector<string>> top_level_category_file_locations = read_burrito_directory(
            path_configs[i],
            &marker_categories,
            &parsed_pois);
        for (map<string, vector<string>>::iterator it = top_level_category_file_locations.begin(); it != top_level_category_file_locations.end(); it++) {
            top_level_category_file_locations_by_pack[it->first].push_back(it->second);
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


    if (duplicate_categories.size() > 0 && parsed_arguments.allow_duplicates == false) {
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
        if (path_configs[i].type != BehaviorType::EXPORT || path_configs[i].format != MarkerFormat::XML) {
            continue;
        }
        write_taco_directory(path_configs[i], &marker_categories, &parsed_pois);
    }
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The xml write function took " << ms << " milliseconds to run" << endl;

    // Write all of the protobin guildpoint paths
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < path_configs.size(); i++) {
        if (path_configs[i].type != BehaviorType::EXPORT || path_configs[i].format != MarkerFormat::GUILDPOINT) {
            continue;
        }
        write_burrito_directory(path_configs[i],&marker_categories, &parsed_pois);
    }
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The protobuf write function took " << ms << " milliseconds to run" << endl;
}


////////////////////////////////////////////////////////////////////////////////
// main
//
// Main is the CLI entrypoint to the xml converter.
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    ParsedArguments parsed_arguments = parse_arguments(argc, argv);
    if (parsed_arguments.path_configs.empty()) {
        return -1;
    }
    process_data(parsed_arguments);

    return 0;
}
