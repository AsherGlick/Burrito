#include "argument_parser.hpp"

#include <cstring>
#include <iostream>
#include <map>

using namespace std;

// Default constructor for the class
PathConfig::PathConfig()
    : type(BehaviorType::NONE),
      format(MarkerFormat::NONE),
      path("DEFAULT"),
      split_by_map_id(false) {
}

PathConfig::PathConfig(BehaviorType type, MarkerFormat format, std::string path, bool split_by_map_id)
    : type(type), format(format), path(path), split_by_map_id(split_by_map_id) {
}

ArgumentConfig::ArgumentConfig(BehaviorType type, MarkerFormat format)
    : type(type), format(format) {
}

////////////////////////////////////////////////////////////////////////////////
// parse_arguments
//
// Processes all of the command line data into a format the internal functions
// want to receive.
////////////////////////////////////////////////////////////////////////////////
ParsedArguments parse_arguments(int argc, char* argv[]) {
    vector<PathConfig> path_configs;
    map<string, ArgumentConfig> arg_map = {
        {"--input-taco-path", ArgumentConfig(BehaviorType::IMPORT, MarkerFormat::XML)},
        {"--output-taco-path", ArgumentConfig(BehaviorType::EXPORT, MarkerFormat::XML)},
        {"--input-guildpoint-path", ArgumentConfig(BehaviorType::IMPORT, MarkerFormat::GUILDPOINT)},
        {"--output-guildpoint-path", ArgumentConfig(BehaviorType::EXPORT, MarkerFormat::GUILDPOINT)}};
    ParsedArguments parsed_arguments;
    BehaviorType type = BehaviorType::NONE;
    MarkerFormat format = MarkerFormat::NONE;
    bool split_by_map_id = false;
    string current_argument;
    vector<string> current_paths;

    for (int i = 1; i < argc; i++) {
        auto it = arg_map.find(argv[i]);
        if (it != arg_map.end()) {
            if (!current_paths.empty()) {
                for (const string& path : current_paths) {
                    path_configs.emplace_back(type, format, path, split_by_map_id);
                }
                current_paths.clear();
            }
            else if (type != BehaviorType::NONE) {
                cerr << "Error: Expected a path to a directory after " << current_argument << endl;
                return {};
            }
            current_argument = argv[i];
            type = it->second.type;
            format = it->second.format;
            // All flags must be set to default value
            split_by_map_id = false;
        }
        else if (!strcmp(argv[i], "--allow-duplicates")) {
            current_argument = argv[i];
            parsed_arguments.allow_duplicates = true;
        }
        else if (!strcmp(argv[i], "--split-by-map-id")) {
            current_argument = argv[i];
            if (type == BehaviorType::IMPORT) {
                cerr << "Error: --split-by-map-id cannot be used after an input argument" << endl;
                return {};
            }
            split_by_map_id = true;
        }
        else {
            current_paths.push_back(argv[i]);
        }
    }

    if (!current_paths.empty()) {
        for (const auto& path : current_paths) {
            path_configs.emplace_back(type, format, path, split_by_map_id);
        }
    }
    parsed_arguments.path_configs = path_configs;
    return parsed_arguments;
}
