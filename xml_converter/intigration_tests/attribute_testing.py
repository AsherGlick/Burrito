import argparse
import difflib
import json
import subprocess
import re
import os
from typing import List, Optional, Final, Tuple

# Path to compiled C++ executable
xml_converter_binary_path: str = "../build/xml_converter"
# Paths to data for the tests
json_file_path: str = "./test_expected_outputs.json"
tests_cases_path: str = "../test_cases"

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


def compare_files(file_path1: str, file_path2: str) -> List[str]:
    with open(file_path1, 'r') as file1, open(file_path2, 'r') as file2:
        content1 = file1.readlines()
        content2 = file2.readlines()

    diff = list(difflib.Differ().compare(content1, content2))
    filtered_diff: List[str] = []
    for line in diff:
        if line.startswith("+ ") or line.startswith("- "):
            filtered_diff.append(line)

    return filtered_diff


patterns_for_noisy_lines = [
    r"^The taco parse function took [0-9]+ milliseconds to run$",
    r"^The xml write function took [0-9]+ milliseconds to run$",
    r"^The protobuf read function took [0-9]+ milliseconds to run$",
    r"^The protobuf write function took [0-9]+ milliseconds to run$",
    r"^$"
]

pattern_for_color_escape_codes = r"\u001b\[[0-9;]+m"


def remove_color_tags(string: str) -> str:
    string = re.sub(pattern_for_color_escape_codes, '', string)
    return string


def remove_noisy_lines(lines: List[str]) -> List[str]:
    filtered_array = []
    for line in lines:
        match_found: bool = False
        for pattern in patterns_for_noisy_lines:
            if re.fullmatch(pattern, line):
                match_found = True
                break
        if not match_found:
            filtered_array.append(remove_color_tags(line))
    return filtered_array


def main() -> None:
    parser = argparse.ArgumentParser(description="A test harness for evaluating the output of the xmlconverter program")
    parser.add_argument("-v", "--verbose", help="Prints the results from xmlconverter in JSON format", action="store_true")
    args = parser.parse_args()

    with open(json_file_path, 'r') as json_file:
        data = json.load(json_file)

    for attribute_data in data:
        attribute_name: str = attribute_data["attribute_name"]
        print(attribute_name)

        input_dir_path = os.path.join(tests_cases_path, attribute_name, "inputs")
        expected_output_dir_path = os.path.join(tests_cases_path, attribute_name, "expected_outputs")
        output_dir_path = os.path.join(tests_cases_path, attribute_name, "outputs")

        # Ensure that the test output directory is created
        os.makedirs(output_dir_path, exist_ok=True)

        for test in attribute_data["tests"]:
            xml_file_name: str = attribute_name + "_" + test["name"] + ".xml"
            input_xml_path = os.path.join(input_dir_path, xml_file_name)
            output_xml_path = os.path.join(output_dir_path, xml_file_name)
            expected_output_xml_path = os.path.join(expected_output_dir_path, test["expected_output_xml_file_name"])

            result = run_xml_converter(input_xml=[input_xml_path], output_xml=[output_xml_path])

            # Remove noisy lines
            stdout: List[str] = remove_noisy_lines(result[0].split("\n"))
            stderr: List[str] = remove_noisy_lines(result[1].split("\n"))
            returncode: int = result[2]

            # Prints the results of xml_converter
            if args.verbose:
                print(f"Test {attribute_name}_{test['name']}")
                print(f"\"stdout\" : {json.dumps(stdout)}")
                print(f"\"stderr\" : {json.dumps(stderr)}")
                print(f"\"return_code\" : {json.dumps(returncode)}")

            all_tests_passed: bool = True
            error_diff: List[str]

            if stdout != test["expected_stdout"]:
                print(f"Standard output did not match for test {attribute_name}{test['name']}")
                error_diff = list(difflib.Differ().compare(test["expected_stdout"], stdout))
                for line in error_diff:
                    print(line)
                all_tests_passed = False

            if stderr != test["expected_stderr"]:
                print(f"Standard error did not match for test {attribute_name}{test['name']}")
                error_diff = list(difflib.Differ().compare(test["expected_stderr"], stderr))
                for line in error_diff:
                    print(line)
                all_tests_passed = False

            if returncode != test["expected_returncode"]:
                print(f"Return code did not match for test {attribute_name}{test['name']}")
                print(f"expected_returncode = {test['expected_returncode']}")
                print(f"returncode = {returncode}")

            xml_diff = compare_files(expected_output_xml_path, output_xml_path)

            if xml_diff != []:
                print(f"Diff was incorrect for test {attribute_name}{test['name']}")
                for line in xml_diff:
                    if line.startswith("+ "):
                        print(line)
                all_tests_passed = False

            if all_tests_passed:
                print(f"Success: test {attribute_name}_{test['name']}")


if __name__ == "__main__":
    main()
