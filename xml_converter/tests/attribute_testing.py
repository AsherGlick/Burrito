import subprocess
import json
import re
import os
from typing import List

# Path to compiled C++ executable
xml_converter: str = "../build/xml_converter"

json_file_path: str = "./test_cases.json"


def run_xml_converter(*args: str) -> str:
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
        return ""


def are_files_equal(file_path1: str, file_path2: str) -> bool:
    try:
        with open(file_path1, 'rb') as file1, open(file_path2, 'rb') as file2:
            content1 = file1.read()
            content2 = file2.read()

        return content1 == content2

    except (FileNotFoundError, PermissionError):
        print(f"Error opening file {file_path1} or {file_path2}")
        return False


def are_arrays_equal(array1: List[str], array2: List[str]) -> bool:
    if len(array1) != len(array2):
        return False

    for i in range(len(array1)):
        if array1[i] != array2[i]:
            return False

    return True


arg_input_xml: str = "--input-taco-path"
arg_output_xml: str = "--output-taco-path"

chrono_patterns = [
    "^The taco parse function took [0-9]+ milliseconds to run$",
    "^The xml write function took [0-9]+ milliseconds to run$",
    "^The protobuf write function took [0-9]+ milliseconds to run$"
]


if __name__ == "__main__":
    try:
        with open(json_file_path, 'r') as json_file:
            data = json.load(json_file)

        for attribute_name in data.keys():
            print(attribute_name)
            attribute_data = data[attribute_name]

            # Ensure that the test output directory is created
            if not os.path.exists(attribute_data["path"] + "-output/"):
                try:
                    os.makedirs(attribute_data["path"] + "-output/")
                except OSError as e:
                    print(f"Error: {e}")

            for test in attribute_data["tests"].keys():
                arg1 = attribute_data["path"] + "/" + attribute_name + test + ".xml"
                arg2 = attribute_data["path"] + "-output/" + attribute_name + test + ".xml"

                result = run_xml_converter(arg_input_xml, arg1, arg_output_xml, arg2)
                if result != "":
                    # Remove lines that state how long it took to run
                    split_result = result.split("\n")
                    filtered_array: List[str] = []
                    for line in split_result:
                        match_found = False
                        if line == "":
                            continue
                        for pattern in chrono_patterns:
                            if re.search(pattern, line):
                                match_found = True
                                break
                        if not match_found:
                            filtered_array.append(line)

                    output_test: bool = True
                    file_test: bool = True
                    expected_results = attribute_data["tests"][test]

                    if are_arrays_equal(filtered_array, expected_results["output_result"]) != expected_results["expect_output_to_be_equal"]:
                        print(f"Output did not match for test {attribute_name}{test}")
                        print(f"Expected {expected_results['output_result']} and got {filtered_array}")
                        output_test = False

                    if are_files_equal(arg1, arg2) != expected_results["expect_files_to_be_equal"]:
                        print(f"Files were incorrect for test {attribute_name}{test}")
                        print(f"Expected {expected_results['expect_files_to_be_equal']}")
                        print(f"arg1: {arg1}")
                        print(f"arg2: {arg2}")
                        file_test = False

                    if output_test and file_test:
                        print(f"Success: test {attribute_name}{test}")

                else:
                    print(f"Failed to execute {attribute_name}{test}")

    except FileNotFoundError:
        print(f"The file '{json_file_path}' does not exist.")
    except json.JSONDecodeError as e:
        print(f"JSON decoding error: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")
