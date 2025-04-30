#!/usr/bin/env python3

import argparse
import difflib
import subprocess
import re
import os
from typing import List, Optional, Tuple, Dict
from src.testcase_loader import load_testcases, Testcase
import shutil
from src.proto_utils import compare_protos, compare_binary_file
import zipfile
from src.trail_utils import compare_trails
from dataclasses import dataclass

# Path to compiled C++ executable
burrito_converter_binary_path: str = "../build/burrito_converter"


@dataclass
class OutputTacoArg:
    path: Optional[str]
    zip_output: bool


@dataclass
class OutputBurritoArg:
    path: Optional[str]
    split_by_map_id: bool
    split_by_category: Optional[int]
    zip_output: bool


def run_burrito_converter(
    output_taco: OutputTacoArg,
    output_burrito: OutputBurritoArg,
    input_taco: Optional[List[str]] = None,
    input_burrito: Optional[List[str]] = None,
    allow_duplicates: Optional[bool] = None,
    verbose: bool = False,
) -> Tuple[str, str, int]:

    # Build the command to execute the C++ program with the desired function and arguments
    cmd: List[str] = [burrito_converter_binary_path]

    if allow_duplicates:
        cmd += ["--allow-duplicates"]
    if input_taco:
        cmd += ["--input-taco-path"] + input_taco
    if output_taco.path:
        cmd += ["--output-taco-path"] + [output_taco.path]
        # TODO: Uncomment when we support zipping output files
        # if output_taco.zip_output:
        #     cmd += "--zip-output"
    if input_burrito:
        cmd += ["--input-guildpoint-path"] + input_burrito
    if output_burrito.path:
        cmd += ["--output-guildpoint-path"] + [output_burrito.path]
        if output_burrito.split_by_map_id:
            cmd += ["--split-by-map-id"]
        if output_burrito.split_by_category is not None:
            cmd += ["--split-by-category"] + [str(output_burrito.split_by_category)]
        # TODO: Uncomment when we support zipping output files
        # if output_burrito.zip_output:
        #     cmd += "--zip-output"

    if verbose:
        print("Converter Command: ", cmd)

    # Run the C++ program and capture its output
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    return (
        result.stdout.decode(encoding="utf-8", errors="backslashreplace"),
        result.stderr.decode(encoding="utf-8", errors="backslashreplace"),
        result.returncode,
    )


def compare_text_files(file_path1: str, file_path2: str) -> List[str]:
    with open(file_path1, 'r') as file1:
        content1 = file1.read().split("\n")
    with open(file_path2, 'r') as file2:
        content2 = file2.read().split("\n")

    diff = list(difflib.unified_diff(content1, content2, fromfile=file_path1, tofile=file_path2, lineterm=""))

    return diff


def len_diff(lines: List[str]) -> int:
    diffcount = 0

    for line in lines:
        if line.startswith(" "):
            continue
        diffcount += 1
    return diffcount


################################################################################
# remove_ansi_color_escapecodes
#
# Remove the ANSI color code escape characters from a string to make it easier
# to read what is actually contained in the string.
################################################################################
pattern_for_color_escape_codes = r"\u001b\[[0-9;]+m"


def remove_ansi_color_escapecodes(lines: List[str]) -> List[str]:
    return [re.sub(pattern_for_color_escape_codes, '', line) for line in lines]


################################################################################
# rebuild_burrito_converter_binary
#
# Recompiles the Burrito Converter binary. If the compilation returns an error code
# then this function throws an error
################################################################################
def rebuild_burrito_converter_binary() -> None:
    print("Building Burrito Converter Binary")
    cmake_build_directory = "../build"

    # Store the current working directory
    original_cwd = os.getcwd()

    # Check if the relative path exists
    target_path = os.path.join(original_cwd, cmake_build_directory)
    if os.path.exists(target_path):
        # Change the working directory to the target directory
        os.chdir(target_path)

        # Run cmake and make
        cmake_process = subprocess.run(["cmake", ".."])
        make_process = subprocess.run(["make"])

        if cmake_process.returncode != 0 or make_process.returncode != 0:
            raise ValueError("Nonzero return code from burrito_converter build process.")

        # Change back to the original working directory
        os.chdir(original_cwd)
    else:
        print(f"Directory '{cmake_build_directory}' does not exist.")

    print()


################################################################################
# remove_ignored_lines
#
# Goes through a list of lines and removes any line that matches a pattern in
# the `line_patterns_to_ignore` global variable.
################################################################################
line_patterns_to_ignore = [
    r"^Loading taco pack .*$",
    r"^Loading guildpoint pack .*$",
    r"^The .+? function took [0-9]+ milliseconds to run$",
    r"^$"
]


def remove_ignored_lines(lines: List[str]) -> List[str]:
    filtered_array = []
    for line in lines:
        match_found: bool = False
        for pattern in line_patterns_to_ignore:
            if re.fullmatch(pattern, line):
                match_found = True
                break
        if not match_found:
            filtered_array.append(line)
    return filtered_array


################################################################################
# zip_directory
#
# Zips and entire directory into a zip file.
################################################################################
def zip_directory(directory_path: str, zip_path: str) -> None:
    with zipfile.ZipFile(zip_path, 'w') as f:
        for root, _, files in os.walk(directory_path):
            for file in files:
                path = os.path.join(root, file)
                relative_filepath = os.path.relpath(path, directory_path)
                f.write(path, relative_filepath)


################################################################################
# find_and_replace
#
# Iterates through each line and replaces anything anything that is a key of
# `find_and_replace_tokens` with the value associated to the key. The longest
# keys are replaced first.
################################################################################
def find_and_replace(lines: List[str], find_and_replace_tokens: Dict[str, str]) -> List[str]:
    new_lines: List[str] = []
    for line in lines:
        for key, value in sorted(find_and_replace_tokens.items(), key=lambda x: -len(x[0])):
            line = line.replace(key, value)
        new_lines.append(line)

    return new_lines


################################################################################
# indent
#
# Adds a 4 space indent to the beginning of each line in the list of lines.
################################################################################
def indent(lines: List[str]) -> List[str]:
    return ["    " + x for x in lines]


################################################################################
# color_unified_diff
#
# Colors a diff using ANSI escape color codes. The diff must be in the
# "unified diff" format."
################################################################################
def color_unified_diff(lines: List[str]) -> List[str]:
    header = True
    colored_lines: List[str] = []
    for line in lines:
        if line[0] == '@':
            colored_lines.append("\033[36m" + line + "\033[0m")
            header = False
        elif header:
            colored_lines.append("\033[39;1m" + line + "\033[0m")
        elif line[0] == '-':
            colored_lines.append("\033[31m" + line + "\033[0m")
        elif line[0] == '+':
            colored_lines.append("\033[32m" + line + "\033[0m")
        else:
            colored_lines.append(line)
    return colored_lines


################################################################################
# zip_inputs
#
# Zips up input files into a new location and returns the new filenames, as
# well as a mapping of filenames to automatically adjust any test output to
# match the zip file paths.
################################################################################
def zip_inputs(
    temp_directory_path: str,
    testcase_name: str,
    xml_input_paths: List[str],
    proto_input_paths: List[str],
) -> Tuple[Dict[str, str], List[str], List[str]]:
    find_and_replace_tokens: Dict[str, str] = {}
    zipped_input_dir = os.path.join(temp_directory_path, "zipped_inputs", testcase_name)
    os.makedirs(zipped_input_dir, exist_ok=True)

    new_xml_input_paths: List[str] = []
    for xml_input_path in xml_input_paths:
        zip_path = os.path.join(zipped_input_dir, os.path.basename(xml_input_path) + ".taco")
        zip_directory(xml_input_path, zip_path)
        new_xml_input_paths.append(zip_path)
        find_and_replace_tokens[xml_input_path] = zip_path

    new_proto_input_paths: List[str] = []
    for proto_input_path in proto_input_paths:
        zip_path = os.path.join(zipped_input_dir, os.path.basename(proto_input_path) + ".burrito")
        zip_directory(proto_input_path, zip_path)
        new_proto_input_paths.append(zip_path)
        find_and_replace_tokens[proto_input_path] = zip_path

    return (
        find_and_replace_tokens,
        new_xml_input_paths,
        new_proto_input_paths,
    )


################################################################################
# run_testcase
#
# Runs a testcase, prints out the output of the testcase and returns if the
# testcase passed.
################################################################################
def run_testcase(
    testcase: Testcase,
    temp_directory_path: str,
    zip_input: bool,
    zip_output: bool,
    verbose: bool,
) -> bool:
    output_xml_path: Optional[str] = None
    output_proto_path: Optional[str] = None

    testcase_display_name = testcase.name

    output_subdirectory = "outputs"
    if zip_input:
        output_subdirectory = "unzipped_outputs"

    if testcase.expected_output_xml_path is not None:
        xml_output_dir_path = os.path.join(temp_directory_path, output_subdirectory, "xml", testcase.name)
        os.makedirs(xml_output_dir_path, exist_ok=True)
        output_xml_path = xml_output_dir_path

    if testcase.expected_output_proto_path is not None:
        proto_output_dir_path = os.path.join(temp_directory_path, output_subdirectory, "proto", testcase.name)
        os.makedirs(proto_output_dir_path, exist_ok=True)
        output_proto_path = proto_output_dir_path

    xml_input_paths = testcase.xml_input_paths
    proto_input_paths = testcase.proto_input_paths

    find_and_replace_tokens: Dict[str, str] = {}

    if zip_input:
        (
            new_find_and_replace_tokens,
            xml_input_paths,
            proto_input_paths
        ) = zip_inputs(
            temp_directory_path,
            testcase.name,
            xml_input_paths,
            proto_input_paths
        )

        find_and_replace_tokens |= new_find_and_replace_tokens
        testcase_display_name = testcase_display_name + " (zipped_inputs)"

    rawstdout, rawstderr, returncode = run_burrito_converter(
        input_taco=xml_input_paths,
        input_burrito=proto_input_paths,
        output_taco=OutputTacoArg(
            output_xml_path,
            zip_output,
        ),
        output_burrito=OutputBurritoArg(
            output_proto_path,
            split_by_map_id=testcase.split_by_map_id,
            split_by_category=testcase.split_by_category,
            zip_output=zip_output,
        ),
        allow_duplicates=testcase.allow_duplicates,
        verbose=verbose,
    )

    # Sanitize and denoise the lines
    stdout: List[str] = remove_ansi_color_escapecodes(remove_ignored_lines(rawstdout.split("\n")))
    stderr: List[str] = remove_ansi_color_escapecodes(remove_ignored_lines(rawstderr.split("\n")))

    # Prints the results of burrito_converter
    if verbose:
        print(f"Test {testcase_display_name}")
        print("    stdout :  {}".format("\n".join(stdout)))
        print("    stderr : {}".format("\n".join(stderr)))
        print("    return_code : {}".format(returncode))

    testcase_passed: bool = True

    # Process standard out
    expected_stdout = find_and_replace(
        lines=testcase.expected_stdout,
        find_and_replace_tokens=find_and_replace_tokens,
    )
    stdout_diff: List[str] = list(difflib.unified_diff(expected_stdout, stdout, fromfile="Expected stdout", tofile="Actual stdout", lineterm=""))
    stdout_diff = color_unified_diff(stdout_diff)
    if len_diff(stdout_diff) != 0:
        print(f"  Standard output did not match for test {testcase_display_name}")
        for line in stdout_diff:
            print("  |" + line)
        testcase_passed = False

    # Process standard error
    expected_stderr = find_and_replace(
        lines=testcase.expected_stderr,
        find_and_replace_tokens=find_and_replace_tokens,
    )
    stderr_diff: List[str] = list(difflib.unified_diff(expected_stderr, stderr, fromfile="Expected stderr", tofile="Actual stderr", lineterm=""))
    stderr_diff = color_unified_diff(stderr_diff)
    if len_diff(stderr_diff) != 0:
        print(f"  Standard error did not match for test {testcase_display_name}")
        for line in stderr_diff:
            print("  |" + line)
        testcase_passed = False

    if testcase.expected_returncode is not None and testcase.expected_returncode != returncode:
        print(f"  Expected a return code of {testcase.expected_returncode} for {testcase_display_name} but got {returncode}")

    if testcase.expected_output_xml_path is not None and os.path.exists(testcase.expected_output_xml_path):
        testcase_passed &= diff_dirs(xml_output_dir_path, testcase.expected_output_xml_path)
    if testcase.expected_output_proto_path is not None and os.path.exists(testcase.expected_output_proto_path):
        testcase_passed &= diff_dirs(proto_output_dir_path, testcase.expected_output_proto_path)

    if testcase_passed:
        print(f"Success: test {testcase_display_name}")
    else:
        print(f"\033[41mFailure\033[0m: test {testcase_display_name}")

    return testcase_passed


################################################################################
# main
#
# Parses the command line arguments, cleans up the test directory, and runs all
# of the tests specified in test_cases.
################################################################################
def main() -> bool:
    parser = argparse.ArgumentParser(description="A test harness for evaluating the output of the xmlconverter program.")
    parser.add_argument("-v", "--verbose", help="Prints the results from xmlconverter in JSON format.", action="store_true")
    parser.add_argument("--filter", help="Filter which tests to run by a regex pattern.", type=str)
    parser.add_argument("--no-build", help="Do not automatically build burrito_converter before running the integration tests.", action="store_true")
    args = parser.parse_args()

    temp_directory_path = ".test_temp"

    # Ensure that the test output directory is empty
    if os.path.exists(temp_directory_path):
        shutil.rmtree(temp_directory_path)

    all_tests_passed = True

    if not args.no_build:
        rebuild_burrito_converter_binary()

    test_run_count = 0

    for testcase in load_testcases():
        if args.filter is not None:
            if not re.match(args.filter, testcase.name):
                continue

        testcase_passed: bool = True

        # TODO: Change the zip_output tuple from (False, ) to (False, True)
        # when we support zipping output files.
        for zip_output in (False, ):
            for zip_input in (False, True):
                testcase_passed &= run_testcase(
                    testcase=testcase,
                    temp_directory_path=temp_directory_path,
                    zip_input=zip_input,
                    zip_output=zip_output,
                    verbose=args.verbose
                )

        all_tests_passed &= testcase_passed
        test_run_count += 1

    if test_run_count == 0:
        print("No Tests Were Run")
        return False

    return all_tests_passed


################################################################################
# diff_dirs
#
# Diffs the file hirearchy and the files themselves between two directories.
################################################################################
def diff_dirs(actual_output_dir: str, expected_output_dir: str) -> bool:
    diff_found = False

    actual_files, actual_directories = get_paths(actual_output_dir)
    expected_files, expected_directories = get_paths(expected_output_dir)

    expected_only_files = set(expected_files) - set(actual_files)
    actual_only_files = set(actual_files) - set(expected_files)

    expected_only_dirs = set(expected_directories) - set(actual_directories)
    actual_only_dirs = set(actual_directories) - set(expected_directories)

    for file in expected_only_files:
        diff_found = True
        print("  |\033[31m-Expected `{}` but did not find the file in the actual output.\033[0m".format(os.path.join(expected_output_dir, file)))
    for file in actual_only_files:
        diff_found = True
        print("  |\033[32m+Unexpected file `{}` found in the actual output.\033[0m".format(os.path.join(actual_output_dir, file)))
    for directory in expected_only_dirs:
        diff_found = True
        print("  |\033[31m-Expected `{}` but did not find the dir in the actual output.\033[0m".format(os.path.join(expected_output_dir, directory)))
    for directory in actual_only_dirs:
        diff_found = True
        print("  |\033[32m+Unexpected dir `{}` found in the actual output.\033[0m".format(os.path.join(actual_output_dir, directory)))

    files_to_diff = set.intersection(set(expected_files), set(actual_files))

    for file_to_diff in files_to_diff:
        expected_file = os.path.join(expected_output_dir, file_to_diff)
        actual_file = os.path.join(actual_output_dir, file_to_diff)

        diff: List[str]
        if file_to_diff.endswith(".xml"):
            diff = compare_text_files(expected_file, actual_file)
        elif file_to_diff.endswith(".guildpoint"):
            diff = compare_protos(actual_file, expected_file)
        elif file_to_diff.endswith(".trl"):
            diff = compare_trails(actual_file, expected_file)
        else:
            diff = []
            if not compare_binary_file(actual_file, expected_file):
                diff = ['{} and {} Files Differ'.format(actual_file, expected_file)]

        if len_diff(diff) != 0:
            diff_found = True
            print("  Output was incorrect for test")
            for line in color_unified_diff(diff):
                print("  |" + line)

    return not diff_found


################################################################################
# get_paths
#
# Gets a list of all of the file and directory paths within a given directory.
# Returns a tuple of `(files, directories)`.
################################################################################
def get_paths(directory: str) -> Tuple[List[str], List[str]]:
    files: List[str] = []
    directories: List[str] = []

    for path in os.listdir(directory):
        if os.path.isdir(os.path.join(directory, path)):
            subfiles, subdirectories = get_paths(os.path.join(directory, path))

            directories.append(path)
            for subfile in subfiles:
                files.append(os.path.join(path, subfile))
            for subdirectory in subdirectories:
                files.append(os.path.join(path, subdirectory))
        else:
            files.append(path)
    return (files, directories)


if __name__ == "__main__":
    all_tests_passed = main()

    # Exit with an error if not all the tests passed so we can catch it in CI
    if not all_tests_passed:
        exit(1)
