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

class CategoryWithinSinglePack {
 public:
    Category* category;
    vector<string> file_paths;

    CategoryWithinSinglePack() {
        this->category = nullptr;
    }
};

struct CategoryWithinMultiplePacks {
    vector<CategoryWithinSinglePack> categories;
};

struct CategoryNameAndFilePath {
    string name;
    string file_path;
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
    map<UniqueId, map<string, vector<CategoryNameAndFilePath>>> id_to_category_name_to_filepaths;
    string directory_name = filesystem::path(input_path).filename();
    vector<MarkerPackFile> xml_files = get_files_by_suffix(input_path, ".xml");
    for (const MarkerPackFile& path : xml_files) {
        map<UniqueId, Category*> top_level_categories = parse_xml_file(path, marker_categories, parsed_pois);
        string file_path = join_file_paths(input_path, path.relative_filepath);
        for (map<UniqueId, Category*>::iterator it = top_level_categories.begin(); it != top_level_categories.end(); it++) {
            UniqueId id = it->first;
            Category* category = it->second;
            id_to_category_name_to_filepaths[id][lowercase(category->name)].push_back({category->name, file_path});
            if (top_level_category_file_locations[id].category == nullptr || top_level_category_file_locations[id].category == category) {
                top_level_category_file_locations[id].category = category;
                top_level_category_file_locations[id].file_paths.push_back(file_path);
            }
        }
    }

    for (auto it = id_to_category_name_to_filepaths.begin(); it != id_to_category_name_to_filepaths.end(); it++) {
        UniqueId id = it->first;
        map<string, vector<CategoryNameAndFilePath>> category_name_to_filepaths = it->second;
        if (category_name_to_filepaths.size() > 1) {
            cerr << "Error: Different MarkerCategory nodes were found with same ID (" << id.get_value_as_base_64() << ")" << endl;
            map<string, vector<string>> category_name_to_file_names;
            for (auto name_to_file_names_it = category_name_to_filepaths.begin(); name_to_file_names_it != category_name_to_filepaths.end(); name_to_file_names_it++) {
                vector<CategoryNameAndFilePath> category_name_and_filepath = name_to_file_names_it->second;
                for (size_t i = 0; i < category_name_and_filepath.size(); i++) {
                    category_name_to_file_names[category_name_and_filepath[i].name].push_back(category_name_and_filepath[i].file_path);
                }
            }
            for (auto names_it = category_name_to_file_names.begin(); names_it != category_name_to_file_names.end(); names_it++) {
                string category_name = names_it->first;
                vector<string> file_paths = names_it->second;
                cerr << "    In these files the Category 'name' is \"" + category_name + "\"" << endl;
                for (size_t i = 0; i < file_paths.size(); i++) {
                    cerr << "        " << file_paths[i] << endl;
                }
            }
        }
    }
    cleanup_xml_files();
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
    map<UniqueId, map<string, vector<CategoryNameAndFilePath>>> id_to_category_name_to_filepaths;
    map<UniqueId, map<string, vector<string>>> id_conflicts;
    string directory_name = filesystem::path(input_path).filename();
    vector<MarkerPackFile> burrito_files = get_files_by_suffix(input_path, ".guildpoint");
    for (const MarkerPackFile& path : burrito_files) {
        map<UniqueId, Category*> top_level_categories = read_protobuf_file(path, marker_categories, parsed_pois);
        string file_path = join_file_paths(input_path, path.relative_filepath);
        for (map<UniqueId, Category*>::iterator it = top_level_categories.begin(); it != top_level_categories.end(); it++) {
            UniqueId id = it->first;
            Category* category = it->second;
            id_to_category_name_to_filepaths[id][lowercase(category->name)].push_back({category->name, file_path});
            if (top_level_category_file_locations[id].category == nullptr || top_level_category_file_locations[id].category == category) {
                top_level_category_file_locations[id].category = category;
                top_level_category_file_locations[id].file_paths.push_back(file_path);
            }
        }
    }
    for (auto it = id_to_category_name_to_filepaths.begin(); it != id_to_category_name_to_filepaths.end(); it++) {
        UniqueId id = it->first;
        map<string, vector<CategoryNameAndFilePath>> category_name_to_filepaths = it->second;
        if (category_name_to_filepaths.size() > 1) {
            cerr << "Error: Different Category messages were found with same ID (" << id.get_value_as_base_64() << ")" << endl;
            map<string, vector<string>> category_name_to_file_names;
            for (auto name_to_file_names_it = category_name_to_filepaths.begin(); name_to_file_names_it != category_name_to_filepaths.end(); name_to_file_names_it++) {
                vector<CategoryNameAndFilePath> category_name_and_filepath = name_to_file_names_it->second;
                for (size_t i = 0; i < category_name_and_filepath.size(); i++) {
                    category_name_to_file_names[category_name_and_filepath[i].name].push_back(category_name_and_filepath[i].file_path);
                }
            }
            for (auto names_it = category_name_to_file_names.begin(); names_it != category_name_to_file_names.end(); names_it++) {
                string category_name = names_it->first;
                vector<string> file_paths = names_it->second;
                cerr << "    In these files the Category 'name' is \"" + category_name + "\"" << endl;
                for (size_t i = 0; i < file_paths.size(); i++) {
                    cerr << "        " << file_paths[i] << endl;
                }
            }
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
// The universal entrypoint into burrito converter functionality. Both the CLI
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
        if (it->second.categories.size() > 1) {
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
        cout << "The following top level categories had a conflict in IDs. For XML, these IDs may be generated from the 'name' attribute." << endl;
        for (map<UniqueId, CategoryWithinMultiplePacks>::iterator it = duplicate_categories.begin(); it != duplicate_categories.end(); it++) {
            cout << "    Categories were found that share the ID \"" << it->first.get_value_as_base_64() << "\"" << endl;
            map<string, vector<string>> files_grouped_by_header;
            for (CategoryWithinSinglePack category_within_single_pack : it->second.categories) {
                for (size_t i = 0; i < category_within_single_pack.file_paths.size(); i++) {
                    string header;
                    if (category_within_single_pack.category->name == "") {
                        header = "In these files no name was found";
                    }
                    else {
                        header = "In these files the Category 'name' is \"" + category_within_single_pack.category->name + "\"";
                    }
                    files_grouped_by_header[header].push_back(category_within_single_pack.file_paths[i]);
                }
            }
            for (pair<string, vector<string>> header_to_file_names : files_grouped_by_header) {
                cout << "        " << header_to_file_names.first << "" << endl;
                for (size_t i = 0; i < header_to_file_names.second.size(); i++) {
                    cout << "            " << header_to_file_names.second[i] << endl;
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
// Main is the CLI entrypoint to burrito converter.
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    ParsedArguments parsed_arguments = parse_arguments(argc, argv);
    if (!parsed_arguments.is_valid) {
        return -1;
    }
    process_data(parsed_arguments);

    return 0;
}
