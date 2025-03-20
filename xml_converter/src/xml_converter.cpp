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

#include "argument_parser.hpp"
#include "attribute/marker_category.hpp"
#include "attribute/unique_id.hpp"
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
using namespace Attribute::UniqueId;

struct CategoryWithinSinglePack {
    Category* category;
    vector<string> file_paths;
};

struct CategoryWithinMultiplePacks {
    vector<CategoryWithinSinglePack> categories;
};

map<UniqueId, CategoryWithinSinglePack> read_taco_directory(
    string input_path,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois
) {
    map<UniqueId, CategoryWithinSinglePack> top_level_category_file_locations;
    if (!filesystem::exists(input_path)) {
        cout << "Error: " << input_path << " is not an existing directory or file" << endl;
    }

    string directory_name = filesystem::path(input_path).filename();
    vector<MarkerPackFile> xml_files = get_files_by_suffix(input_path, ".xml");
    for (const MarkerPackFile& path : xml_files) {
        map<UniqueId, Category*> top_level_categories = parse_xml_file(path, marker_categories, parsed_pois);
        string file_path = join_file_paths(input_path, path.relative_filepath);
        for (map<UniqueId, Category*>::iterator it = top_level_categories.begin(); it != top_level_categories.end(); it++) {
            if (top_level_category_file_locations.find(it->first) != top_level_category_file_locations.end()) {
                if (top_level_category_file_locations[it->first].category != it->second) {
                    cerr << "Error: Different MarkerCategory objects were found with same ID" << endl;
                    for (size_t i = 0; i < top_level_category_file_locations[it->first].file_paths.size(); i++) {
                        cerr << "    \"" << top_level_category_file_locations[it->first].category->name << "\" in file " << top_level_category_file_locations[it->first].file_paths[i] << endl;
                    }
                    cerr << "    \"" << it->second->name << "\" in file " << file_path << endl;
                    continue;
                }
            }
            else {
                top_level_category_file_locations[it->first].category = it->second;
            }
            top_level_category_file_locations[it->first].file_paths.push_back(file_path);
        }
    }

    return top_level_category_file_locations;
}

map<UniqueId, CategoryWithinSinglePack> read_burrito_directory(
    string input_path,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois
) {
    map<UniqueId, CategoryWithinSinglePack> top_level_category_file_locations;
    if (!filesystem::exists(input_path)) {
        cout << "Error: " << input_path << " is not an existing directory or file" << endl;
    }

    string directory_name = filesystem::path(input_path).filename();
    vector<MarkerPackFile> burrito_files = get_files_by_suffix(input_path, ".guildpoint");
    for (const MarkerPackFile& path : burrito_files) {
        map<UniqueId, Category*> top_level_categories = read_protobuf_file(path, marker_categories, parsed_pois);
        string file_path = join_file_paths(input_path, path.relative_filepath);
        for (map<UniqueId, Category*>::iterator it = top_level_categories.begin(); it != top_level_categories.end(); it++) {
            if (top_level_category_file_locations.find(it->first) != top_level_category_file_locations.end()) {
                if (top_level_category_file_locations[it->first].category != it->second) {
                    cerr << "Error: Different MarkerCategory objects were found with same ID" << endl;
                    for (size_t i = 0; i < top_level_category_file_locations[it->first].file_paths.size(); i++) {
                        cerr << "    \"" << top_level_category_file_locations[it->first].category->name << "\" in file: " << top_level_category_file_locations[it->first].file_paths[i] << endl;
                    }
                    cerr << "    \"" << it->second->name << "\" in file: " << file_path << endl;
                    continue;
                }
            }
            else {
                top_level_category_file_locations[it->first].category = it->second;
            }
            top_level_category_file_locations[it->first].file_paths.push_back(file_path);
        }
    }

    return top_level_category_file_locations;
}

void write_taco_directory(
    string input_path,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois
) {
    // TODO: Exportion of XML Marker Packs File Structure #111
    if (!filesystem::is_directory(input_path)) {
        if (!filesystem::create_directory(input_path)) {
            cout << "Error: " << input_path << "is not a valid directory path" << endl;
            return;
        }
    }
    write_xml_file(input_path, marker_categories, parsed_pois);
}

void write_burrito_directory(
    string input_path,
    bool split_by_map_id,
    OptionalInt split_by_category_depth,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois
) {
    if (!filesystem::is_directory(input_path)) {
        if (!filesystem::create_directory(input_path)) {
            cout << "Error: " << input_path << "is not a valid directory path" << endl;
            return;
        }
    }
    StringHierarchy category_filter;
    category_filter.add_path({}, true);
    if (split_by_map_id && split_by_category_depth.has_value()) {
        cerr << "Error: 'split by category' and 'split by map id' can't be used at the same time" << endl;
        return;
    }
    if (split_by_map_id) {
        write_protobuf_file_per_map_id(input_path, category_filter, marker_categories, parsed_pois);
    }
    else if (split_by_category_depth.has_value()) {
        write_protobuf_file_per_category(input_path, split_by_category_depth.get_value(), marker_categories, parsed_pois);
    }
    else {
        write_protobuf_file(input_path, category_filter, marker_categories, parsed_pois);
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
    map<UniqueId, CategoryWithinMultiplePacks> top_level_category_file_locations_by_pack;
    map<UniqueId, CategoryWithinMultiplePacks> duplicate_categories;
    vector<MarkerPackConfig> marker_pack_config = parsed_arguments.marker_pack_configs;

    // Read in all the xml taco markerpacks
    auto begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < marker_pack_config.size(); i++) {
        if (marker_pack_config[i].type != BehaviorType::IMPORT || marker_pack_config[i].format != MarkerFormat::XML) {
            continue;
        }
        cout << "Loading taco pack " << marker_pack_config[i].path << endl;

        map<UniqueId, CategoryWithinSinglePack> top_level_category_file_locations = read_taco_directory(
            marker_pack_config[i].path,
            &marker_categories,
            &parsed_pois
        );
        for (map<UniqueId, CategoryWithinSinglePack>::iterator it = top_level_category_file_locations.begin(); it != top_level_category_file_locations.end(); it++) {
            top_level_category_file_locations_by_pack[it->first].categories.push_back(it->second);
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The taco parse function took " << ms << " milliseconds to run" << endl;

    // Read in all the protobin guildpoint markerpacks
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < marker_pack_config.size(); i++) {
        if (marker_pack_config[i].type != BehaviorType::IMPORT || marker_pack_config[i].format != MarkerFormat::GUILDPOINT) {
            continue;
        }
        cout << "Loading guildpoint pack " << marker_pack_config[i].path << endl;

        map<UniqueId, CategoryWithinSinglePack> top_level_category_file_locations = read_burrito_directory(
            marker_pack_config[i].path,
            &marker_categories,
            &parsed_pois
        );
        for (map<UniqueId, CategoryWithinSinglePack>::iterator it = top_level_category_file_locations.begin(); it != top_level_category_file_locations.end(); it++) {
            top_level_category_file_locations_by_pack[it->first].categories.push_back(it->second);
        }
    }
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The guildpoint parse function took " << ms << " milliseconds to run" << endl;

    for (map<UniqueId, CategoryWithinMultiplePacks>::iterator it = top_level_category_file_locations_by_pack.begin(); it != top_level_category_file_locations_by_pack.end(); it++) {
        if (it->second.categories.size() != 1) {
            duplicate_categories[it->first] = it->second;
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
        for (map<UniqueId, CategoryWithinMultiplePacks>::iterator it = duplicate_categories.begin(); it != duplicate_categories.end(); it++) {
            for (CategoryWithinSinglePack category_within_single_pack : it->second.categories) {
                if (category_within_single_pack.category->display_name_is_set) {
                    cout << "    \"" << category_within_single_pack.category->display_name << "\" in files:" << endl;
                }
                else {
                    cout << "    \"" << category_within_single_pack.category->name << "\" in files:" << endl;
                }
                for (size_t i = 0; i < category_within_single_pack.file_paths.size(); i++) {
                    cout << "        " << category_within_single_pack.file_paths[i] << endl;
                }
            }
        }
        return;
    }

    // Write all of the xml taco paths
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < marker_pack_config.size(); i++) {
        if (marker_pack_config[i].type != BehaviorType::EXPORT || marker_pack_config[i].format != MarkerFormat::XML) {
            continue;
        }
        write_taco_directory(marker_pack_config[i].path, &marker_categories, &parsed_pois);
    }
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The xml write function took " << ms << " milliseconds to run" << endl;

    // Write all of the protobin guildpoint paths
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < marker_pack_config.size(); i++) {
        if (marker_pack_config[i].type != BehaviorType::EXPORT || marker_pack_config[i].format != MarkerFormat::GUILDPOINT) {
            continue;
        }
        write_burrito_directory(marker_pack_config[i].path, marker_pack_config[i].split_by_map_id, marker_pack_config[i].split_by_category, &marker_categories, &parsed_pois);
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
    if (!parsed_arguments.is_valid) {
        return -1;
    }
    process_data(parsed_arguments);

    return 0;
}
