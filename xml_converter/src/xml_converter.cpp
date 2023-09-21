#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
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
#include "icon_gen.hpp"
#include "packaging_protobin.hpp"
#include "packaging_xml.hpp"
#include "parseable.hpp"
#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "string_helper.hpp"
#include "trail_gen.hpp"
#include "waypoint.pb.h"

using namespace std;

bool filename_comp(string a, string b) {
    return lowercase(a) < lowercase(b);
}

vector<string> get_files_by_suffix(string directory, string suffix) {
    vector<string> files;
    DIR* dir = opendir(directory.c_str());
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        string filename = entry->d_name;
        if (filename != "." && filename != "..") {
            string path = directory + "/" + filename;
            if (entry->d_type == DT_DIR) {
                vector<string> subfiles = get_files_by_suffix(path, suffix);
                // Default: markerpacks have all xml files in the first directory
                for (string subfile : subfiles) {
                    cout << subfile << " found in subfolder" << endl;
                    files.push_back(subfile);
                }
            }
            else if (has_suffix(filename, suffix)) {
                files.push_back(path);
            }
        }
    }
    closedir(dir);
    std::sort(files.begin(), files.end(), filename_comp);
    return files;
}

void move_supplementary_files(string input_directory, string output_directory) {
    DIR* dir = opendir(input_directory.c_str());
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        string filename = entry->d_name;
        if (filename != "." && filename != "..") {
            string path = input_directory + "/" + filename;
            if (entry->d_type == DT_DIR) {
                string new_directory = output_directory + "/" + filename;
                if (mkdir(new_directory.c_str(), 0700) == -1 && errno != EEXIST) {
                    cout << "Error making " << new_directory << endl;
                    continue;
                }
                move_supplementary_files(path, new_directory);
            }
            else if (has_suffix(filename, ".trl") || has_suffix(filename, ".xml")) {
                continue;
            }
            else {
                // TODO: Only include files that are referenced by the
                // individual markers in order to avoid any unnessecary files
                string new_path = output_directory + "/" + filename;
                copy_file(path, new_path);
            }
        }
    }
}

void read_taco_directory(string directory, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    vector<string> xml_files = get_files_by_suffix(directory, ".xml");
    for (const string& path : xml_files) {
        parse_xml_file(path, marker_categories, parsed_pois);
    }
}

void write_taco_directory(string directory, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    // TODO: Exportion of XML Marker Packs File Structure #111
    string xml_filepath = directory + "xml_file.xml";
    write_xml_file(xml_filepath, marker_categories, parsed_pois);
}

////////////////////////////////////////////////////////////////////////////////
// process_data
//
// The universal entrypoint into the xml converter functionality. Both the CLI
// and the library entrypoints direct here to do their actual processing.
////////////////////////////////////////////////////////////////////////////////
void process_data(
    vector<string> input_taco_paths,
    vector<string> input_waypoint_paths,

    // These will eventually have additional arguments for each output path to
    // allow for splitting out a single markerpack
    vector<string> output_taco_paths,
    vector<string> output_waypoint_paths,

    // This is a special output path used for burrito internal use that splits
    // the waypoint protobins by map id.
    string output_split_waypoint_dir) {
    // All of the loaded pois and categories
    vector<Parseable*> parsed_pois;
    map<string, Category> marker_categories;

    // Read in all the xml taco markerpacks
    auto begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < input_taco_paths.size(); i++) {
        cout << "Loading taco pack " << input_taco_paths[i] << endl;
        read_taco_directory(
            input_taco_paths[i],
            &marker_categories,
            &parsed_pois);

        // TODO: This is wildly incorrect now because we might have a
        //       different output directory then output_split_waypoint_dir
        if (output_split_waypoint_dir != "") {
            move_supplementary_files(input_taco_paths[i], output_split_waypoint_dir);
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The taco parse function took " << ms << " milliseconds to run" << endl;

    // Read in all the protobin waypoint markerpacks
    for (size_t i = 0; i < input_waypoint_paths.size(); i++) {
        cout << "Loading waypoint pack " << input_waypoint_paths[i] << endl;
        read_protobuf_file(
            input_waypoint_paths[i],
            &marker_categories,
            &parsed_pois);
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

    // Write all of the protobin waypoint paths
    for (size_t i = 0; i < output_waypoint_paths.size(); i++) {
        write_protobuf_file(output_waypoint_paths[i], &marker_categories, &parsed_pois);
    }

    // Write the special map-split protbin waypoint file
    begin = chrono::high_resolution_clock::now();
    if (output_split_waypoint_dir != "") {
        write_protobuf_file(output_split_waypoint_dir, &marker_categories, &parsed_pois);
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
//   ./xml_converter --input-taco-paths ../packs/marker_pack --output-split-waypoint-path ../output_packs
//   ./xml_converter --input-taco-paths ../packs/* --output-split-waypoint-path ../output_packs
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    vector<string> input_taco_paths;
    vector<string> output_taco_paths;
    vector<string> input_waypoint_paths;
    vector<string> output_waypoint_paths;

    // Typically "~/.local/share/godot/app_userdata/Burrito/protobins" for
    // converting from xml markerpacks to internal protobuf files.
    vector<string> output_split_waypoint_paths;

    vector<string>* arg_target = &input_taco_paths;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--input-taco-path")) {
            arg_target = &input_taco_paths;
        }
        else if (!strcmp(argv[i], "--output-taco-path")) {
            arg_target = &output_taco_paths;
        }
        else if (!strcmp(argv[i], "--input-waypoint-path")) {
            arg_target = &input_waypoint_paths;
        }
        else if (!strcmp(argv[i], "--output-waypoint-path")) {
            arg_target = &output_waypoint_paths;
        }
        else if (!strcmp(argv[i], "--output-split-waypoint-path")) {
            // We dont actually support multiple values for this argument but
            // I am leaving this as-is because it is simpler. We can adjust the
            // CLI arg parsing later to properly capture this.
            arg_target = &output_split_waypoint_paths;
        }
        else {
            arg_target->push_back(argv[i]);
        }
    }

    // Strip all but the first output split waypoint argument, because we dont
    // actually support multiple arguments.
    string output_split_waypoint_dir = "";
    if (output_split_waypoint_paths.size() > 0) {
        output_split_waypoint_dir = output_split_waypoint_paths[0];
    }
    else if (output_split_waypoint_paths.size() > 1) {
        cout << "Only one --output-split-waypoint-path is accepted" << endl;
    }

    process_data(
        input_taco_paths,
        input_waypoint_paths,
        output_taco_paths,
        output_waypoint_paths,
        output_split_waypoint_dir);

    return 0;
}
