from dataclasses import dataclass
import argparse
import difflib
import json
import subprocess
import re
import os
from typing import List, Optional

# Path to compiled C++ executable
xml_converter: str = "../build/xml_converter"
json_file_path: str = "../test_cases/test_expected_outputs.json"


@dataclass
class XMLConverter_arguments:
    arg_input_xml: str = "--input-taco-path"
    arg_output_xml: str = "--output-taco-path"
    arg_input_proto: str = "--input-waypoint-path"
    arg_output_proto: str = "--output-waypoint-path"
    arg_split_proto: str = "--output-split-waypoint-path"


def run_xml_converter(
    input_xml: Optional[List[str]] = None,
    output_xml: Optional[List[str]] = None,
    input_proto: Optional[List[str]] = None,
    output_proto: Optional[List[str]] = None,
    split_output_proto: Optional[str] = None,
) -> subprocess.CompletedProcess[str]:

    # Build the command to execute the C++ program with the desired function and arguments
    cmd: List[str] = [xml_converter]

    if input_xml:
        cmd += [XMLConverter_arguments.arg_input_xml] + input_xml
    if output_xml:
        cmd += [XMLConverter_arguments.arg_output_xml] + output_xml
    if input_proto:
        cmd += [XMLConverter_arguments.arg_input_proto] + input_proto
    if output_proto:
        cmd += [XMLConverter_arguments.arg_output_proto] + output_proto
    if split_output_proto:
        cmd += [XMLConverter_arguments.arg_split_proto] + [split_output_proto]

    # Run the C++ program and capture its output
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    return result


def compare_files(file_path1: str, file_path2: str) -> List[str]:
    try:
        with open(file_path1, 'r') as file1, open(file_path2, 'r') as file2:
            content1 = file1.readlines()
            content2 = file2.readlines()

        differ = difflib.Differ()
        diff = list(differ.compare(content1, content2))
        filtered_diff: List[str] = []
        for line in diff:
            if line.startswith("+ ") or line.startswith("- "):
                filtered_diff.append(line)

        return filtered_diff

    except (FileNotFoundError, PermissionError) as e:
        print(f"Error opening file {file_path1} or {file_path2}")
        print(e)
        return [str(e)]


patterns_for_noisy_lines = [
    "^The taco parse function took [0-9]+ milliseconds to run$",
    "^The xml write function took [0-9]+ milliseconds to run$",
    "^The protobuf read function took [0-9]+ milliseconds to run$",
    "^The protobuf write function took [0-9]+ milliseconds to run$",
    "^$"
]


def remove_noisy_lines(array: List[str]) -> List[str]:
    filtered_array = []
    for line in array:
        match_found: bool = False
        for pattern in patterns_for_noisy_lines:
            if re.fullmatch(pattern, line):
                match_found = True
                break
        if not match_found:
            filtered_array.append(line)
    return filtered_array


def main(args: argparse.Namespace) -> None:
    try:
        with open(json_file_path, 'r') as json_file:
            data = json.load(json_file)

        for attribute_name in data.keys():
            print(attribute_name)
            attribute_data = data[attribute_name]

            # Ensure that the test output directory is created
            output_dir_path = attribute_data["path"] + "-output/"
            if not os.path.exists(output_dir_path):
                try:
                    os.makedirs(output_dir_path)
                except OSError as e:
                    print(f"Error: {e}")

            for test in attribute_data["tests"].keys():
                file_name: str = attribute_name + test + ".xml"
                input_xml_path = os.path.join(attribute_data["path"], file_name)
                output_xml_path = os.path.join(output_dir_path, file_name)

                result = run_xml_converter(input_xml=[input_xml_path], output_xml=[output_xml_path])
                # Remove noisy lines
                stdout = remove_noisy_lines(result.stdout.split("\n"))
                stderr = remove_noisy_lines(result.stderr.split("\n"))
                xml_diff = compare_files(input_xml_path, output_xml_path)

                # Prints the results rather than comparing them to a file
                if args.print:
                    print(f"Test {attribute_name}{test}")
                    print(f"'output_stdout' : {json.dumps(stdout)}")
                    print(f"'output_stderr' = {json.dumps(stderr)}")
                    print(f"'expected_xml_diff' = {json.dumps(xml_diff)}")
                    continue

                all_tests_passed: bool = True
                expected_results = attribute_data["tests"][test]

                if stdout != expected_results["output_stdout"]:
                    print(f"Output did not match for test {attribute_name}{test}")
                    print(f"Expected stdout {expected_results['output_stdout']} and got {stdout}")
                    all_tests_passed = False

                if stderr != expected_results["output_stderr"]:
                    print(f"Output did not match for test {attribute_name}{test}")
                    print(f"Expected stderr {expected_results['output_stderr']} and got {stderr}")
                    all_tests_passed = False

                if xml_diff != expected_results["expected_xml_diff"]:
                    print(f"Diff was incorrect for test {attribute_name}{test}")
                    print(f"Expected {expected_results['expected_xml_diff']} and got {xml_diff}")
                    all_tests_passed = False

                if all_tests_passed:
                    print(f"Success: test {attribute_name}{test}")

    except FileNotFoundError:
        print(f"The file '{json_file_path}' does not exist.")
    except json.JSONDecodeError as e:
        print(f"JSON decoding error: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="An example script with an optional argument.")
    parser.add_argument("-p", "--print", help="Prints the results rather than comparing them to a file", action="store_true")

    args = parser.parse_args()
    main(args)
