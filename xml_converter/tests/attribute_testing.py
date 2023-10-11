from dataclasses import dataclass
import subprocess
import re
import os
from typing import Dict, List

# Path to compiled C++ executable
xml_converter: str = "../build/xml_converter"


@dataclass
class Attribute_Tests:
    attribute_name: str
    attribute_path: str
    tests: List[str]
    output_result: Dict[str, List[str]]
    expect_output_to_be_equal: Dict[str, bool]
    expect_files_to_be_equal: Dict[str, bool]


def run_xml_converter(*args):
    try:
        # Build the command to execute the C++ program with the desired function and arguments
        cmd = [xml_converter] + list(args)

        # Run the C++ program and capture its output
        result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        # Check if the C++ program executed successfully
        if result.returncode == 0:
            return result.stdout
        else:
            return result.stderr
    except Exception as e:
        print("Error:", e)
        return None


def are_files_equal(file_path1, file_path2) -> bool:
    try:
        with open(file_path1, 'rb') as file1, open(file_path2, 'rb') as file2:
            content1 = file1.read()
            content2 = file2.read()

        return content1 == content2

    except (FileNotFoundError, PermissionError):
        print(f"Error opening file {file_path1} or {file_path2}")
        return False


def are_arrays_equal(array1, array2) -> bool:
    if len(array1) != len(array2):
        return False

    for i in range(len(array1)):
        if array1[i] != array2[i]:
            return False

    return True


arg_input_xml: str = "--input-taco-path"
arg_output_xml: str = "--output-taco-path"

chrono_patterns = [
    "The taco parse function took [0-9]+ milliseconds to run",
    "The xml write function took [0-9]+ milliseconds to run",
    "The protobuf write function took [0-9]+ milliseconds to run"
]


if __name__ == "__main__":
    # TODO: This will need to be extracted as more attributes and tests are added
    attribute: Attribute_Tests = Attribute_Tests(
        attribute_name="can_fade",
        attribute_path="../test_cases/can_fade_tests",
        tests=["_is_correct", "_is_different_file", "_is_wrong_output"],
        output_result={
            "_is_correct": ['Loading taco pack ../test_cases/can_fade_tests/can_fade_is_correct.xml'],
            "_is_different_file": ['Loading taco pack ../test_cases/can_fade_tests/can_fade_is_different_file.xml'],
            "_is_wrong_output": ['WRONG']
        },
        expect_output_to_be_equal={
            "_is_correct": True,
            "_is_different_file": True,
            "_is_wrong_output": False,
        },
        expect_files_to_be_equal={
            "_is_correct": True,
            "_is_different_file": False,
            "_is_wrong_output": True,
        }
    )

    # Ensure that the test output directory is created
    if not os.path.exists(attribute.attribute_path + "-output/"):
        try:
            os.makedirs(attribute.attribute_path + "-output/")
        except OSError as e:
            print(f"Error: {e}")

    for test in attribute.tests:
        arg1 = attribute.attribute_path + "/" + attribute.attribute_name + test + ".xml"
        arg2 = attribute.attribute_path + "-output/" + attribute.attribute_name + test + ".xml"

        result = run_xml_converter(arg_input_xml, arg1, arg_output_xml, arg2)
        if result is not None:
            # Remove lines about how long it took to run
            split_result = result.split("\n")
            filtered_array = []
            for line in split_result:
                match_found = False
                for pattern in chrono_patterns:
                    if re.search(pattern, line) or line == "":
                        match_found = True
                        break
                if not match_found:
                    filtered_array.append(line)

            output_test: bool = True
            file_test: bool = True

            if are_arrays_equal(filtered_array, attribute.output_result[test]) != attribute.expect_output_to_be_equal[test]:
                print(f"Output did not match for test {attribute.attribute_name}{test}")
                print(f"Expected {attribute.output_result[test]} and got {filtered_array}")
                output_test = False

            if are_files_equal(arg1, arg2) != attribute.expect_files_to_be_equal[test]:
                print(f"Files were incorrect for test {attribute.attribute_name}{test}")
                print(f"Expected {attribute.expect_files_to_be_equal[test]}")
                print(f"arg1: {arg1}")
                print(f"arg2: {arg2}")
                file_test = False

            if output_test and file_test:
                print(f"Successful test {attribute.attribute_name}{test}")

        else:
            print(f"Failed to execute {attribute.attribute_name}{test}")
