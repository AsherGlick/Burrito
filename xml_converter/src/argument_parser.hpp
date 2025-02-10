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

class MarkerPackConfig {
 public:
    BehaviorType type;
    MarkerFormat format;
    std::string path;
    bool split_by_map_id = false;

    MarkerPackConfig();

    MarkerPackConfig(BehaviorType type, MarkerFormat format, std::string path, bool split_by_map_id = false);
};

class ParsedArguments {
 public:
    std::vector<MarkerPackConfig> marker_pack_configs;
    bool allow_duplicates = false;
    bool is_valid = false;
};

ParsedArguments parse_arguments(int argc, char* argv[]);
