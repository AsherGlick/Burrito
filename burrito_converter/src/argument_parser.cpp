#include "argument_parser.hpp"

#include <cstring>
#include <iostream>

#include "int_helper.hpp"

using namespace std;

InputTacoCommand::InputTacoCommand(string path)
    : path(path) {
}

InputGuildpointCommand::InputGuildpointCommand(string path)
    : path(path) {
}

OutputTacoCommand::OutputTacoCommand(string path)
    : path(path) {
}

OutputGuildpointCommand::OutputGuildpointCommand(
    std::string path,
    OptionalInt split_by_category_depth,
    bool split_by_map_id
)
    : path(path),
      split_by_category_depth(split_by_category_depth),
      split_by_map_id(split_by_map_id) {
}

////////////////////////////////////////////////////////////////////////////////
// ParsedArgReturn
//
// A helper struct which works as a return type to return a list of commands
// alongside some additional parsing information
////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct ParsedArgReturn {
    vector<T*> command;
    unsigned int arguments_parsed;
    bool error;
};

////////////////////////////////////////////////////////////////////////////////
// parse_input_taco_args
//
// All of the logic for parsing the --input-taco-path argument, and any
// associated arguments it might have.
////////////////////////////////////////////////////////////////////////////////
ParsedArgReturn<InputTacoCommand> parse_input_taco_args(unsigned int argc, char* argv[]) {
    // Figure out how many path arguments there are for this command
    unsigned int number_of_path_args = 0;
    while (number_of_path_args < argc && string(argv[number_of_path_args]).find("--") != 0) {
        number_of_path_args++;
    }
    if (number_of_path_args < 1) {
        cerr << "Error: --input-taco-path is missing a path" << endl;
        return {{}, 0, true};
    }

    // Build all of the commands
    vector<InputTacoCommand*> commands;
    for (unsigned int i = 0; i < number_of_path_args; i++) {
        commands.push_back(
            new InputTacoCommand(
                string(argv[i])
            )
        );
    }

    return {
        commands,
        number_of_path_args,
        false,
    };
}

////////////////////////////////////////////////////////////////////////////////
// parse_input_guildpoint_args
//
// All of the logic for parsing the --input-guildpoint-path argument, and any
// associated arguments it might have.
////////////////////////////////////////////////////////////////////////////////
ParsedArgReturn<InputGuildpointCommand> parse_input_guildpoint_args(unsigned int argc, char* argv[]) {
    // Figure out how many path arguments there are for this command
    unsigned int number_of_path_args = 0;
    while (number_of_path_args < argc && string(argv[number_of_path_args]).find("--") != 0) {
        number_of_path_args++;
    }
    if (number_of_path_args < 1) {
        cerr << "Error: --input-guildpoint-path is missing a path" << endl;
        return {{}, 0, true};
    }

    // Build all of the commands
    vector<InputGuildpointCommand*> commands;
    for (unsigned int i = 0; i < number_of_path_args; i++) {
        commands.push_back(
            new InputGuildpointCommand(
                string(argv[i])
            )
        );
    }

    return {
        commands,
        number_of_path_args,
        false,
    };
}

////////////////////////////////////////////////////////////////////////////////
// parse_output_taco_args
//
// All of the logic for parsing the --output-taco-path argument, and any
// associated arguments it might have.
////////////////////////////////////////////////////////////////////////////////
ParsedArgReturn<OutputTacoCommand> parse_output_taco_args(unsigned int argc, char* argv[]) {
    // Figure out how many path arguments there are for this command
    unsigned int number_of_path_args = 0;
    while (number_of_path_args < argc && string(argv[number_of_path_args]).find("--") != 0) {
        number_of_path_args++;
    }
    if (number_of_path_args < 1) {
        cerr << "Error: --output-taco-path is missing a path" << endl;
        return {{}, 0, true};
    }

    // Build all of the commands
    vector<OutputTacoCommand*> commands;
    for (unsigned int i = 0; i < number_of_path_args; i++) {
        commands.push_back(
            new OutputTacoCommand(
                string(argv[i])
            )
        );
    }

    return {
        commands,
        number_of_path_args,
        false,
    };
}

////////////////////////////////////////////////////////////////////////////////
// parse_output_guildpoint_args
//
// All of the logic for parsing the --output-guildpoint-path argument, and any
// associated arguments it might have.
////////////////////////////////////////////////////////////////////////////////
ParsedArgReturn<OutputGuildpointCommand> parse_output_guildpoint_args(unsigned int argc, char* argv[]) {
    // Figure out how many path arguments there are for this command
    unsigned int number_of_path_args = 0;
    while (number_of_path_args < argc && string(argv[number_of_path_args]).find("--") != 0) {
        number_of_path_args++;
    }
    if (number_of_path_args < 1) {
        cerr << "Error: --output-guildpoint-path is missing a path" << endl;
        return {{}, 0, true};
    }

    // Parse the other arguments for this command
    OptionalInt split_by_category_depth;
    bool split_by_map_id = false;

    unsigned int total_args_parsed;
    for (total_args_parsed = number_of_path_args; total_args_parsed < argc; total_args_parsed++) {
        if (string(argv[total_args_parsed]) == "--split-by-category") {
            split_by_category_depth.set_value(0);
            if (total_args_parsed + 1 < argc && string(argv[total_args_parsed + 1]).find("--") != 0) {
                if (is_string_valid_integer(argv[total_args_parsed + 1])) {
                    split_by_category_depth.set_value(std::stoi(argv[++total_args_parsed]));
                }
                else {
                    cerr << "Error: expected an integer after --split-by-category but received " << argv[total_args_parsed + 1] << endl;
                    return {{}, 0, true};
                }
            }
        }
        else if (string(argv[total_args_parsed]) == "--split-by-map-id") {
            split_by_map_id = true;
        }
        else {
            break;
        }
    }

    // Build all of the commands
    vector<OutputGuildpointCommand*> commands;
    for (unsigned int i = 0; i < number_of_path_args; i++) {
        commands.push_back(
            new OutputGuildpointCommand(
                string(argv[i]),
                split_by_category_depth,
                split_by_map_id
            )
        );
    }

    return {
        commands,
        total_args_parsed,
        false,
    };
}

////////////////////////////////////////////////////////////////////////////////
// parse_arguments
//
// Processes all of the command line data into a format the internal functions
// want to receive.
////////////////////////////////////////////////////////////////////////////////
ParsedArguments parse_arguments(int argc, char* argv[]) {
    std::vector<BaseCommand*> marker_pack_configs;
    ParsedArguments parsed_arguments;

    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "--input-taco-path") {
            ParsedArgReturn<InputTacoCommand> result = parse_input_taco_args(argc - 1 - i, argv + 1 + i);
            if (result.error) return {};
            marker_pack_configs.insert(marker_pack_configs.end(), result.command.begin(), result.command.end());
            i += result.arguments_parsed;
        }

        else if (string(argv[i]) == "--input-guildpoint-path") {
            ParsedArgReturn<InputGuildpointCommand> result = parse_input_guildpoint_args(argc - 1 - i, argv + 1 + i);
            if (result.error) return {};
            marker_pack_configs.insert(marker_pack_configs.end(), result.command.begin(), result.command.end());
            i += result.arguments_parsed;
        }

        else if (string(argv[i]) == "--output-taco-path") {
            ParsedArgReturn<OutputTacoCommand> result = parse_output_taco_args(argc - 1 - i, argv + 1 + i);
            if (result.error) return {};
            marker_pack_configs.insert(marker_pack_configs.end(), result.command.begin(), result.command.end());
            i += result.arguments_parsed;
        }

        else if (string(argv[i]) == "--output-guildpoint-path") {
            ParsedArgReturn<OutputGuildpointCommand> result = parse_output_guildpoint_args(argc - 1 - i, argv + 1 + i);
            if (result.error) return {};
            marker_pack_configs.insert(marker_pack_configs.end(), result.command.begin(), result.command.end());
            i += result.arguments_parsed;
        }

        else if (string(argv[i]) == "--allow-duplicates") {
            parsed_arguments.allow_duplicates = true;
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

    parsed_arguments.marker_pack_configs = marker_pack_configs;
    parsed_arguments.is_valid = true;
    return parsed_arguments;
}

////////////////////////////////////////////////////////////////////////////////
// ~ParsedArguments
//
// Clean up all of the allocated marker pack commands when the parsed argument
// object is deconstructed. TODO
////////////////////////////////////////////////////////////////////////////////
ParsedArguments::~ParsedArguments() {
    // for (size_t i = 0; i < this->marker_pack_configs.size(); i++) {
    //     delete marker_pack_configs[i];
    // }
}
