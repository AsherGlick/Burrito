#pragma once

#include <string>
#include <vector>

// Defines what behavior is expected i.e. (Read/Import or Write/Export)
enum class BehaviorType {
    IMPORT,
    EXPORT,
    NONE,
};

// Defines what format the data is expected to be in
enum class MarkerFormat {
    XML,
    GUILDPOINT,
    NONE,
};

class PathConfig {
 public:
    BehaviorType type;
    MarkerFormat format;
    std::string path;
    bool split_by_map_id = false;

    PathConfig();

    PathConfig(BehaviorType type, MarkerFormat format, std::string path, bool split_by_map_id = false);
};

class ArgumentConfig {
 public:
    BehaviorType type;
    MarkerFormat format;

    ArgumentConfig(BehaviorType type, MarkerFormat format);
};

class ParsedArguments {
 public:
    std::vector<PathConfig> path_configs;
    bool allow_duplicates = false;
};

ParsedArguments parse_arguments(int argc, char* argv[]);
