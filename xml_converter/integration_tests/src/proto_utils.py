import os
import subprocess
import difflib
from typing import List


def compare_protos(
    expected_proto_path: str,
    actual_proto_path: str
) -> List[str]:
    files_are_equal = compare_binary_file(expected_proto_path, actual_proto_path)

    if files_are_equal:
        return []

    expected_textproto = get_guildpoint_textproto(expected_proto_path)
    actual_textproto = get_guildpoint_textproto(actual_proto_path)

    diff = list(difflib.unified_diff(actual_textproto.split("\n"), expected_textproto.split("\n"), fromfile=actual_proto_path, tofile=expected_proto_path, lineterm=""))

    if len(diff) == 0:
        diff = ["Something went wrong diffing {} and {}.".format(expected_proto_path, actual_proto_path)]

    return list(diff)


def compare_binary_file(file_path_1: str, file_path_2: str) -> bool:
    if not os.path.exists(file_path_1):
        return False
    if not os.path.exists(file_path_2):
        return False

    with open(file_path_1, 'rb') as file1:
        contents_1 = file1.read()
    with open(file_path_2, 'rb') as file2:
        contents_2 = file2.read()

    return contents_1 == contents_2


################################################################################
# get_guildpoint_textproto
#
# Reads a guildpoint protobin and returns a stringy textproto value of the
# contents of the protobin. This makes it easier to diff the contents but also
# can be used to easily inspect the values of the protobin.
################################################################################
def get_guildpoint_textproto(protobin_path: str) -> str:
    proto_schema_path = "../proto/guildpoint.proto"
    proto_schema_basedir = "../proto"

    if not os.path.exists(protobin_path):
        return ""

    with open(protobin_path, 'rb') as f:
        result = subprocess.run(
            [
                "protoc",
                "--decode=guildpoint.Guildpoint",
                "--proto_path=" + proto_schema_basedir,
                proto_schema_path
            ],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            stdin=f,
        )

        # TODO: sanity check return code, stdout, and stderr

        return result.stdout.decode("utf-8")
