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

    expected_output_xml_path: Optional[str]
    expected_output_proto_path: Optional[str]

    expected_stdout: List[str]
    expected_stderr: List[str]
    expected_returncode: int
    allow_duplicates: bool
    split_by_map_id: bool
    split_by_category: Optional[int]


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
    allow_duplicates: bool = False
    split_by_map_id: bool = False
    split_by_category: Optional[int] = None
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

    expected_output_xml_path: Optional[str] = os.path.join(path, "output_xml")
    expected_output_proto_path: Optional[str] = os.path.join(path, "output_proto")

    if "exclude_output" in data:
        for pack_type in data["exclude_output"]:
            if pack_type == "xml":
                expected_output_xml_path = None
            if pack_type == "proto":
                expected_output_proto_path = None
            else:
                print(f"Invalid pack type {pack_type} found in exclude_output in {test_info_path}")
                return None

    if "allow_duplicates" in data:
        if not isinstance(data["allow_duplicates"], bool):
            print(f"Invalid Test, expecting bool value for 'allow_duplicates' in {path}")
            return None
        else:
            allow_duplicates = data["allow_duplicates"]

    if "split_by_map_id" in data:
        if not isinstance(data["split_by_map_id"], bool):
            print(f"Invalid Test, expecting bool value for 'split_by_map_id' in {path}")
            return None
        else:
            split_by_map_id = data["split_by_map_id"]

    if "split_by_category" in data:
        if not isinstance(data["split_by_category"], int):
            print(f"Invalid Test, expecting int value for 'split_by_category' in {path}")
            return None
        else:
            split_by_category = data["split_by_category"]

    return Testcase(
        name=os.path.basename(path),
        xml_input_paths=xml_input_paths,
        proto_input_paths=proto_input_paths,
        expected_output_xml_path=expected_output_xml_path,
        expected_output_proto_path=expected_output_proto_path,
        expected_stdout=to_lines(data["expected_stdout"]),
        expected_stderr=to_lines(data["expected_stderr"]),
        expected_returncode=data["expected_returncode"],
        allow_duplicates=allow_duplicates,
        split_by_map_id=split_by_map_id,
        split_by_category=split_by_category,
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
