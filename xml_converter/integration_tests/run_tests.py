#!/usr/bin/env python3

import argparse
import difflib
import subprocess
import re
import os
from typing import List, Optional, Tuple
from src.testcase_loader import load_testcases
import shutil
from src.proto_utils import compare_protos, compare_binary_file

# Path to compiled C++ executable
xml_converter_binary_path: str = "../build/xml_converter"


def run_xml_converter(
    input_xml: Optional[List[str]] = None,
    output_xml: Optional[List[str]] = None,
    input_proto: Optional[List[str]] = None,
    output_proto: Optional[List[str]] = None,
    split_output_proto: Optional[str] = None,
) -> Tuple[str, str, int]:

    # Build the command to execute the C++ program with the desired function and arguments
    cmd: List[str] = [xml_converter_binary_path]

    if input_xml:
        cmd += ["--input-taco-path"] + input_xml
    if output_xml:
        cmd += ["--output-taco-path"] + output_xml
    if input_proto:
        cmd += ["--input-waypoint-path"] + input_proto
    if output_proto:
        cmd += ["--output-waypoint-path"] + output_proto
    if split_output_proto:
        cmd += ["--output-split-waypoint-path"] + [split_output_proto]

    # Run the C++ program and capture its output
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    return (result.stdout, result.stderr, result.returncode)


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
# remove_ansii_color_escapecodes
#
# Remove the ANSII color code escape characters from a string to make it easier
# to read what is actually contained in the string.
################################################################################
pattern_for_color_escape_codes = r"\u001b\[[0-9;]+m"


def remove_ansii_color_escapecodes(lines: List[str]) -> List[str]:
    return [re.sub(pattern_for_color_escape_codes, '', line) for line in lines]


################################################################################
# rebuild_xml_converter_binary
#
# Recompiles the XML Converter binary. If the compilation returns an error code
# then this function throws an error
################################################################################
def rebuild_xml_converter_binary() -> None:
    print("Building XML Converter Binary")
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
            raise ValueError("Nonzero return code from xml_converter build process.")

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
    r"^The taco parse function took [0-9]+ milliseconds to run$",
    r"^The xml write function took [0-9]+ milliseconds to run$",
    r"^The protobuf read function took [0-9]+ milliseconds to run$",
    r"^The protobuf write function took [0-9]+ milliseconds to run$",
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


def main() -> bool:
    parser = argparse.ArgumentParser(description="A test harness for evaluating the output of the xmlconverter program.")
    parser.add_argument("-v", "--verbose", help="Prints the results from xmlconverter in JSON format.", action="store_true")
    parser.add_argument("--filter", help="Filter which tests to run by a regex pattern.", type=str)
    args = parser.parse_args()

    output_parent_dirpath = "./outputs"

    # Ensure that the test output directory is empty
    if os.path.exists(output_parent_dirpath):
        shutil.rmtree(output_parent_dirpath)

    all_tests_passed = True

    rebuild_xml_converter_binary()

    test_run_count = 0

    for testcase in load_testcases():
        if args.filter is not None:
            if not re.match(args.filter, testcase.name):
                continue

        xml_output_dir_path = os.path.join(output_parent_dirpath, "xml", testcase.name)
        proto_output_dir_path = os.path.join(output_parent_dirpath, "proto", testcase.name)

        os.makedirs(xml_output_dir_path, exist_ok=True)
        os.makedirs(proto_output_dir_path, exist_ok=True)

        rawstdout, rawstderr, returncode = run_xml_converter(
            input_xml=testcase.xml_input_paths,
            output_xml=[xml_output_dir_path],
            output_proto=[proto_output_dir_path],
        )

        # Sanitize and denoise the lines
        stdout: List[str] = remove_ansii_color_escapecodes(remove_ignored_lines(rawstdout.split("\n")))
        stderr: List[str] = remove_ansii_color_escapecodes(remove_ignored_lines(rawstderr.split("\n")))

        # Prints the results of xml_converter
        if args.verbose:
            print(f"Test {testcase.name}")
            print("    stdout :  {}".format("\n".join(stdout)))
            print("    stderr : {}".format("\n".join(stderr)))
            print("    return_code : {}".format(returncode))

        testcase_passed: bool = True

        stdout_diff: List[str] = list(difflib.unified_diff(testcase.expected_stdout, stdout, fromfile="Expected stdout", tofile="Actual stdout", lineterm=""))
        if len_diff(stdout_diff) != 0:
            print(f"Standard output did not match for test {testcase.name}")
            for line in stdout_diff:
                print(line)
            testcase_passed = False

        stderr_diff: List[str] = list(difflib.unified_diff(testcase.expected_stderr, stderr, fromfile="Expected stderr", tofile="Actual stderr", lineterm=""))
        if len_diff(stderr_diff) != 0:
            print(f"Standard error did not match for test {testcase.name}")
            for line in stderr_diff:
                print(line)
            testcase_passed = False

        if testcase.expected_returncode is not None and testcase.expected_returncode != returncode:
            print(f"Expected a return code of {testcase.expected_returncode} for {testcase.name} but got {returncode}")

        testcase_passed &= diff_dirs(xml_output_dir_path, testcase.expected_output_xml_path)
        testcase_passed &= diff_dirs(proto_output_dir_path, testcase.expected_output_proto_path)

        if testcase_passed:
            print(f"Success: test {testcase.name}")

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
        print("-Expected `{}` but did not find the file in the actual output.".format(os.path.join(expected_output_dir, file)))
    for file in actual_only_files:
        diff_found = True
        print("+Unexpected file `{}` found in the actual output.".format(os.path.join(actual_output_dir, file)))
    for directory in expected_only_dirs:
        diff_found = True
        print("-Expected `{}` but did not find the dir in the actual output.".format(os.path.join(expected_output_dir, directory)))
    for directory in actual_only_dirs:
        diff_found = True
        print("+Unexpected dir `{}` found in the actual output.".format(os.path.join(actual_output_dir, directory)))

    files_to_diff = set.intersection(set(expected_files), set(actual_files))

    for file_to_diff in files_to_diff:
        expected_file = os.path.join(expected_output_dir, file_to_diff)
        actual_file = os.path.join(actual_output_dir, file_to_diff)

        diff: List[str]
        if file_to_diff.endswith(".xml"):
            diff = compare_text_files(expected_file, actual_file)
        elif file_to_diff.endswith(".data") or file_to_diff.endswith(".bin") or file_to_diff.endswith(".guildp"):
            diff = compare_protos(actual_file, expected_file)
        elif file_to_diff.endswith(".trl"):
            diff = []
            if not compare_binary_file(actual_file, expected_file):
                diff = ['{} and {} Files Differ'.format(actual_file, expected_file)]
        else:
            diff = []
            if not compare_binary_file(actual_file, expected_file):
                diff = ['{} and {} Files Differ'.format(actual_file, expected_file)]

        if len_diff(diff) != 0:
            diff_found = True
            print("Output was incorrect for test")
            for line in diff:
                print(line)

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
