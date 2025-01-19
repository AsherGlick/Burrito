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
        vector<MarkerPackFile> xml_files = get_files_by_suffix(input_path, ".xml");
        for (const MarkerPackFile& path : xml_files) {
            set<string> top_level_category_names = parse_xml_file(path.tmp_get_path(), input_path, marker_categories, parsed_pois);
            string relative_path = join_file_paths(directory_name, path.relative_filepath);
            for (set<string>::iterator it = top_level_category_names.begin(); it != top_level_category_names.end(); it++) {
                top_level_category_file_locations[*it].push_back(relative_path);
            }
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
        vector<MarkerPackFile> burrito_files = get_files_by_suffix(input_path, ".guildpoint");
        for (const MarkerPackFile& path : burrito_files) {
            set<string> top_level_category_names = read_protobuf_file(path.tmp_get_path(), input_path, marker_categories, parsed_pois);
            string relative_path = join_file_paths(directory_name, path.relative_filepath);
            for (set<string>::iterator it = top_level_category_names.begin(); it != top_level_category_names.end(); it++) {
                top_level_category_file_locations[*it].push_back(relative_path);
            }
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
    write_protobuf_file(output_path, category_filter, marker_categories, parsed_pois);
}

////////////////////////////////////////////////////////////////////////////////
// process_data
//
// The universal entrypoint into the xml converter functionality. Both the CLI
// and the library entrypoints direct here to do their actual processing.
////////////////////////////////////////////////////////////////////////////////
void process_data(
    vector<string> input_taco_paths,
    vector<string> input_guildpoint_paths,
    // If multiple inputs are found to have the same top level categories,
    // The program will skip writing to output unless the below is true
    bool allow_duplicates,
    // These will eventually have additional arguments for each output path to
    // allow for splitting out a single markerpack
    vector<string> output_taco_paths,
    vector<string> output_guildpoint_paths,

    // This is a special output path used for burrito internal use that splits
    // the guildpoint protobins by map id.
    string output_split_guildpoint_dir) {
    // All of the loaded pois and categories
    vector<Parseable*> parsed_pois;
    map<string, Category> marker_categories;
    map<string, vector<vector<string>>> top_level_category_file_locations_by_pack;
    map<string, set<string>> duplicate_categories;

    // Read in all the xml taco markerpacks
    auto begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < input_taco_paths.size(); i++) {
        cout << "Loading taco pack " << input_taco_paths[i] << endl;

        map<string, vector<string>> top_level_category_file_locations = read_taco_directory(
            input_taco_paths[i],
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
    for (size_t i = 0; i < input_guildpoint_paths.size(); i++) {
        cout << "Loading guildpoint pack " << input_guildpoint_paths[i] << endl;

        map<string, vector<string>> top_level_category_file_locations = read_burrito_directory(
            input_guildpoint_paths[i],
            &marker_categories,
            &parsed_pois);
        for (map<string, vector<string>>::iterator it = top_level_category_file_locations.begin(); it != top_level_category_file_locations.end(); it++) {
            top_level_category_file_locations_by_pack[it->first].push_back(it->second);
        }
    }

    for (map<string, vector<vector<string>>>::iterator it = top_level_category_file_locations_by_pack.begin(); it != top_level_category_file_locations_by_pack.end(); it++) {
        if (it->second.size() != 1) {
            for (size_t i = 0; i < it->second.size(); i++) {
                for (size_t j = 0; j < it->second[i].size(); j++) {
                    duplicate_categories[it->first].insert(it->second[i][j]);
                }
            }
        }
    }
    if (duplicate_categories.size() > 0 && allow_duplicates == false) {
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
    for (size_t i = 0; i < output_taco_paths.size(); i++) {
        write_taco_directory(output_taco_paths[i], &marker_categories, &parsed_pois);
    }
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The xml write function took " << ms << " milliseconds to run" << endl;

    // Write all of the protobin guildpoint paths
    for (size_t i = 0; i < output_guildpoint_paths.size(); i++) {
        write_burrito_directory(output_guildpoint_paths[i], &marker_categories, &parsed_pois);
    }

    // Write the special map-split protbin guildpoint file
    begin = chrono::high_resolution_clock::now();
    if (output_split_guildpoint_dir != "") {
        StringHierarchy category_filter;
        category_filter.add_path({}, true);
        write_protobuf_file_per_map_id(output_split_guildpoint_dir, category_filter, &marker_categories, &parsed_pois);
    }
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The protobuf write function took " << ms << " milliseconds to run" << endl;
}

////////////////////////////////////////////////////////////////////////////////
// main
//
// Main is the CLI entrypoint to the xml converter. It handles processing all
// of the command line data into a format the internal functions want to
// receive.
//
// Example usage
//   ./xml_converter --input-taco-paths ../packs/marker_pack --output-split-guildpoint-path ../output_packs
//   ./xml_converter --input-taco-paths ../packs/* --output-split-guildpoint-path ../output_packs
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    vector<string> input_taco_paths;
    vector<string> output_taco_paths;
    vector<string> input_guildpoint_paths;
    vector<string> output_guildpoint_paths;
    bool allow_duplicates = false;

    // Typically "~/.local/share/godot/app_userdata/Burrito/protobins" for
    // converting from xml markerpacks to internal protobuf files.
    vector<string> output_split_guildpoint_paths;

    vector<string>* arg_target = nullptr;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--input-taco-path")) {
            arg_target = &input_taco_paths;
        }
        else if (!strcmp(argv[i], "--output-taco-path")) {
            arg_target = &output_taco_paths;
        }
        else if (!strcmp(argv[i], "--input-guildpoint-path")) {
            arg_target = &input_guildpoint_paths;
        }
        else if (!strcmp(argv[i], "--output-guildpoint-path")) {
            arg_target = &output_guildpoint_paths;
        }
        else if (!strcmp(argv[i], "--output-split-guildpoint-path")) {
            // We dont actually support multiple values for this argument but
            // I am leaving this as-is because it is simpler. We can adjust the
            // CLI arg parsing later to properly capture this.
            arg_target = &output_split_guildpoint_paths;
        }
        else if (!strcmp(argv[i], "--allow-duplicates")) {
            allow_duplicates = true;
            arg_target = nullptr;
        }
        else {
            if (arg_target != nullptr) {
                arg_target->push_back(argv[i]);
            }
            else {
                cout << "Unknown argument " << argv[i] << endl;
                return -1;
            }
        }
    }

    // Strip all but the first output split guildpoint argument, because we dont
    // actually support multiple arguments.
    string output_split_guildpoint_dir = "";
    if (output_split_guildpoint_paths.size() > 0) {
        output_split_guildpoint_dir = output_split_guildpoint_paths[0];
    }
    else if (output_split_guildpoint_paths.size() > 1) {
        cout << "Only one --output-split-guildpoint-path is accepted" << endl;
    }

    process_data(
        input_taco_paths,
        input_guildpoint_paths,
        allow_duplicates,
        output_taco_paths,
        output_guildpoint_paths,
        output_split_guildpoint_dir);

    return 0;
}
