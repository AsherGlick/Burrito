#pragma once

#include <string>
#include <vector>

#include "int_helper.hpp"

////////////////////////////////////////////////////////////////////////////////
// BaseCommand
//
// The baseclass for each command struct. Used as a virtual class to allow for
// polymorphism in the parsed data.
////////////////////////////////////////////////////////////////////////////////
class BaseCommand {
 public:
    virtual ~BaseCommand() = default;
};

////////////////////////////////////////////////////////////////////////////////
// InputTacoCommand
//
// All of the arguments associated with --input-taco-path command
////////////////////////////////////////////////////////////////////////////////
class InputTacoCommand : public BaseCommand {
 public:
    explicit InputTacoCommand(std::string path);
    std::string path;
};

////////////////////////////////////////////////////////////////////////////////
// InputGuildpointCommand
//
// All of the arguments associated with the --input-guildpoint-path command
////////////////////////////////////////////////////////////////////////////////
class InputGuildpointCommand : public BaseCommand {
 public:
    explicit InputGuildpointCommand(std::string path);
    std::string path;
};

////////////////////////////////////////////////////////////////////////////////
// OutputTacoCommand
//
// All of the arguments associated with the --output-taco-path command
////////////////////////////////////////////////////////////////////////////////
class OutputTacoCommand : public BaseCommand {
 public:
    explicit OutputTacoCommand(std::string path);
    std::string path;
};

////////////////////////////////////////////////////////////////////////////////
// OutputGuildpointCommand
//
// All of the arguments associated with the --output-guildpoint-path command
////////////////////////////////////////////////////////////////////////////////
class OutputGuildpointCommand : public BaseCommand {
 public:
    OutputGuildpointCommand(std::string path, OptionalInt split_by_category_depth, bool split_by_map_id);
    std::string path;
    OptionalInt split_by_category_depth;
    bool split_by_map_id;
    bool output_full_category_list;
};

////////////////////////////////////////////////////////////////////////////////
// ParsedArguments
//
// Class that stores all of the information about all of the arguments that
// were passed in on the command line, parsed into a cleaner format.
////////////////////////////////////////////////////////////////////////////////
class ParsedArguments {
 public:
    std::vector<BaseCommand*> marker_pack_configs;
    bool allow_duplicates = false;
    bool is_valid = false;

    ~ParsedArguments();
};

ParsedArguments parse_arguments(int argc, char* argv[]);
