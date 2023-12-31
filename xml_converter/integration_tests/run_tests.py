#!/usr/bin/env python3

import argparse
import difflib
import subprocess
import re
import os
from typing import List, Optional, Final, Tuple
from src.testcase_loader import load_testcases
import shutil
from src.proto_utils import compare_protos

# Path to compiled C++ executable
xml_converter_binary_path: str = "../build/xml_converter"

arg_input_xml: Final[str] = "--input-taco-path"
arg_output_xml: Final[str] = "--output-taco-path"
arg_input_proto: Final[str] = "--input-waypoint-path"
arg_output_proto: Final[str] = "--output-waypoint-path"
arg_split_proto: Final[str] = "--output-split-waypoint-path"


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
        cmd += [arg_input_xml] + input_xml
    if output_xml:
        cmd += [arg_output_xml] + output_xml
    if input_proto:
        cmd += [arg_input_proto] + input_proto
    if output_proto:
        cmd += [arg_output_proto] + output_proto
    if split_output_proto:
        cmd += [arg_split_proto] + [split_output_proto]

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


def main() -> None:
    parser = argparse.ArgumentParser(description="A test harness for evaluating the output of the xmlconverter program")
    parser.add_argument("-v", "--verbose", help="Prints the results from xmlconverter in JSON format", action="store_true")
    args = parser.parse_args()

    output_parent_dirpath = "./outputs"

    # Ensure that the test output directory is empty
    if os.path.exists(output_parent_dirpath):
        shutil.rmtree(output_parent_dirpath)

    rebuild_xml_converter_binary()

    for testcase in load_testcases():
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

        all_tests_passed: bool = True

        stdout_diff: List[str] = list(difflib.unified_diff(testcase.expected_stdout, stdout, fromfile="Expected stdout", tofile="Actual stdout", lineterm=""))
        if len_diff(stdout_diff) != 0:
            print(f"Standard output did not match for test {testcase.name}")
            for line in stdout_diff:
                print(line)
            all_tests_passed = False

        stderr_diff: List[str] = list(difflib.unified_diff(testcase.expected_stderr, stderr, fromfile="Expected stderr", tofile="Actual stderr", lineterm=""))
        if len_diff(stderr_diff) != 0:
            print(f"Standard error did not match for test {testcase.name}")
            for line in stderr_diff:
                print(line)
            all_tests_passed = False

        if testcase.expected_returncode is not None and testcase.expected_returncode != returncode:
            print(f"Expected a return code of {testcase.expected_returncode} for {testcase.name} but got {returncode}")

        if testcase.expected_output_xml_path is not None:
            # TODO: These paths are directories and `xml_file.xml` is just one
            # possible file in the directories. Eventually we should check all
            # the files in the directory not just the one.
            output_xml_filepath = os.path.join(xml_output_dir_path, "xml_file.xml")
            expected_output_xml_filepath = os.path.join(testcase.expected_output_xml_path, "xml_file.xml")

            xml_diff = compare_text_files(expected_output_xml_filepath, output_xml_filepath)

            if len_diff(xml_diff) != 0:
                print(f"XML output was incorrect for test {testcase.name}")
                for line in xml_diff:
                    print(line)
                all_tests_passed = False

        if testcase.expected_output_proto_path is not None:
            protos_are_equal = compare_protos(
                outputs_directory=proto_output_dir_path,
                expected_outputs_directory=testcase.expected_output_proto_path,
            )

            all_tests_passed &= protos_are_equal

        if all_tests_passed:
            print(f"Success: test {testcase.name}")


if __name__ == "__main__":
    main()
