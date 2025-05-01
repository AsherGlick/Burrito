#include "argument_parser.hpp"

#include <cstring>
#include <iostream>
#include <map>

using namespace std;

// Default constructor for the class
MarkerPackConfig::MarkerPackConfig()
    : type(BehaviorType::NONE),
      format(MarkerFormat::NONE),
      path("DEFAULT"),
      split_by_map_id(false) {
}

MarkerPackConfig::MarkerPackConfig(BehaviorType type, MarkerFormat format, std::string path, OptionalInt split_by_category, bool split_by_map_id)
    : type(type), format(format), path(path), split_by_category(split_by_category), split_by_map_id(split_by_map_id) {
}

class ArgumentConfig {
 public:
    BehaviorType type;
    MarkerFormat format;

    ArgumentConfig(BehaviorType type, MarkerFormat format)
        : type(type), format(format) {
    }
};

////////////////////////////////////////////////////////////////////////////////
// parse_arguments
//
// Processes all of the command line data into a format the internal functions
// want to receive.
////////////////////////////////////////////////////////////////////////////////
ParsedArguments parse_arguments(int argc, char* argv[]) {
    vector<MarkerPackConfig> marker_pack_configs;
    map<string, ArgumentConfig> arg_map = {
        {"--input-taco-path", ArgumentConfig(BehaviorType::IMPORT, MarkerFormat::XML)},
        {"--output-taco-path", ArgumentConfig(BehaviorType::EXPORT, MarkerFormat::XML)},
        {"--input-guildpoint-path", ArgumentConfig(BehaviorType::IMPORT, MarkerFormat::GUILDPOINT)},
        {"--output-guildpoint-path", ArgumentConfig(BehaviorType::EXPORT, MarkerFormat::GUILDPOINT)}};
    ParsedArguments parsed_arguments;
    BehaviorType type = BehaviorType::NONE;
    MarkerFormat format = MarkerFormat::NONE;
    OptionalInt split_by_category_depth;
    bool split_by_map_id = false;
    string current_argument;
    vector<string> current_paths;

    for (int i = 1; i < argc; i++) {
        auto it = arg_map.find(argv[i]);
        if (it != arg_map.end()) {
            if (!current_paths.empty()) {
                for (const string& path : current_paths) {
                    marker_pack_configs.emplace_back(type, format, path, split_by_category_depth, split_by_map_id);
                }
                current_paths.clear();
            }
            current_argument = argv[i];
            type = it->second.type;
            format = it->second.format;
            // All flags must be set to default value
            split_by_category_depth.set_null();
            split_by_map_id = false;

            while (i + 1 < argc && string(argv[i + 1]).find("--") != 0) {
                current_paths.push_back(argv[++i]);
            }
            if (current_paths.empty()) {
                cerr << "Error: Expected a path to a directory after " << current_argument << endl;
                return {};
            }
        }
        else if (string(argv[i]) == "--allow-duplicates") {
            parsed_arguments.allow_duplicates = true;
        }
        else if (string(argv[i]) == "--split-by-category") {
            if (type != BehaviorType::EXPORT) {
                cerr << "Error: --split-by-category needs to follow an output argument" << endl;
                return {};
            }
            split_by_category_depth.set_value(0);
            if (i + 1 < argc && string(argv[i + 1]).find("--") != 0) {
                if (is_string_valid_integer(argv[i + 1])) {
                    split_by_category_depth.set_value(std::stoi(argv[++i]));
                }
                else {
                    cerr << "Error: expected an integer after --split-by-category but received " << argv[i + 1] << endl;
                    return {};
                }
            }
        }
        else if (string(argv[i]) == "--split-by-map-id") {
            if (type != BehaviorType::EXPORT) {
                cerr << "Error: --split-by-map-id needs to follow an output argument" << endl;
                return {};
            }
            split_by_map_id = true;
        }
        else if (string(argv[i]) == "--help") {
            cout << "usage: ./burrito_converter [--help] [--allow-duplicates]" << endl;
            cout << "                       [--input-taco-path <PATH> ...]" << endl;
            cout << "                       [--input-guildpoint=path <PATH> ...]" << endl;
            cout << "                       [--output-taco-path <PATH> ...]" << endl;
            cout << "                       [--output-guildpoint-path <PATH> [--split-by-map-id] [--split-by-category [DEPTH]] ...]" << endl;
            cout << "Each PATH value should be an absolute or relative path to a marker pack directory or file" << endl;
            cout << "Supported file types include .burrito, .taco, and .zip" << endl;
            return {};
        }
        else {
            cerr << "Error: Unknown argument " << argv[i] << endl;
            return {};
        }
    }

    if (!current_paths.empty()) {
        for (const auto& path : current_paths) {
            marker_pack_configs.emplace_back(type, format, path, split_by_category_depth, split_by_map_id);
        }
    }

    parsed_arguments.marker_pack_configs = marker_pack_configs;
    parsed_arguments.is_valid = true;
    return parsed_arguments;
}
