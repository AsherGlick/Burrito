from typing import List, Optional
from dataclasses import dataclass
import yaml
import os


################################################################################
# Testcase
#
# A dataclass for storing all the structured data for a single testcase.
################################################################################
@dataclass
class Testcase:
    name: str
    xml_input_paths: List[str]
    proto_input_paths: List[str]

    expected_output_xml_path: str
    expected_output_proto_path: str

    expected_stdout: List[str]
    expected_stderr: List[str]
    expected_returncode: int


################################################################################
# load_testcases
#
# Load all of the testcases found in the `test_cases` directory.
################################################################################
def load_testcases() -> List[Testcase]:
    testcase_dirs: List[Testcase] = []

    for testcase_dir in os.listdir("test_cases"):
        testcase = load_testcase(os.path.join("test_cases", testcase_dir))

        if testcase is None:
            continue

        testcase_dirs.append(testcase)

    return testcase_dirs


################################################################################
# load_testcase
#
# A simple loader that loads the testcase from a specific directory, doing
# typechecking on the testcase data to be sure it is properly structured.
################################################################################
def load_testcase(path: str) -> Optional[Testcase]:
    test_info_path = os.path.join(path, "testcase.yaml")
    with open(test_info_path) as f:
        data = yaml.safe_load(f)

    inputs_path = os.path.join(path, "input")

    # Load all of the input paths into either xml input or proto inputs
    xml_input_paths: List[str] = []
    proto_input_paths: List[str] = []
    for pack_name, pack_type in data["input_paths"].items():
        if not isinstance(pack_name, str):
            print(f"Invalid pack name, expecting a string but got {pack_name}")
            return None

        pack_path = os.path.join(inputs_path, pack_name)

        if not os.path.exists(pack_path):
            print(f"Input pack path {pack_path} not found")
            return None

        if pack_type == "xml":
            xml_input_paths.append(pack_path)
        elif pack_type == "proto":
            proto_input_paths.append(pack_path)
        else:
            print(f"Invalid pack type {pack_type} found in {test_info_path}")
            return None

    # Sanity check that all the input directories were accounted for
    for possible_input_path in os.listdir(inputs_path):
        if possible_input_path not in data["input_paths"]:
            print(f"Found the input directory {possible_input_path} in {path} but no config for it")

    # Typecheck the expected stdout, stderr, and returncode values
    if "expected_stdout" not in data:
        print(f"Expected 'expected_stdout' field in {test_info_path}")
        return None
    elif not isinstance(data["expected_stdout"], str):
        print(f"Invalid Test, expecting string value for 'expected_stdout' in {path}")
        return None

    if "expected_stderr" not in data:
        print(f"Expected 'expected_stderr' field in {test_info_path}")
        return None
    elif not isinstance(data["expected_stderr"], str):
        print(f"Invalid Test, expecting string value for 'expected_stderr' in {path}")
        return None

    if "expected_returncode" not in data:
        print(f"Expected 'expected_returncode' field in {test_info_path}")
        return None
    elif not isinstance(data["expected_returncode"], int):
        print(f"Invalid Test, expecting string value for 'expected_returncode' in {path}")
        return None

    return Testcase(
        name=os.path.basename(path),
        xml_input_paths=xml_input_paths,
        proto_input_paths=proto_input_paths,
        expected_output_xml_path=os.path.join(path, "output_xml"),
        expected_output_proto_path=os.path.join(path, "output_proto"),
        expected_stdout=to_lines(data["expected_stdout"]),
        expected_stderr=to_lines(data["expected_stderr"]),
        expected_returncode=data["expected_returncode"]
    )


################################################################################
# to_lines
#
# A helper function to remove final empty lines from split arrays.
################################################################################
def to_lines(value: str) -> List[str]:
    output = value.split("\n")
    if output[-1] == "":
        return output[:-1]
    else:
        return output
