import os
import subprocess
import difflib


def compare_protos(
    outputs_directory: str,
    expected_outputs_directory: str,
) -> bool:
    # TODO: These paths are directories and 'markers.bin` is just one
    # possible file in the directories. Eventually we should check all
    # the files in the directory not just the one.
    files_are_equal = compare_binary_file(
        os.path.join(expected_outputs_directory, "markers.bin"),
        os.path.join(outputs_directory, "markers.bin"),
    )

    if files_are_equal:
        return True

    expected_textproto_path = os.path.join(expected_outputs_directory, "markers.bin")
    actual_textproto_path = os.path.join(outputs_directory, "markers.bin")

    expected_textproto = get_waypoint_textproto(expected_textproto_path)
    actual_textproto = get_waypoint_textproto(actual_textproto_path)

    diff = difflib.unified_diff(expected_textproto.split("\n"), actual_textproto.split("\n"), fromfile=expected_textproto_path, tofile=actual_textproto_path, lineterm="")

    for line in diff:
        print(line)

    # TODO: Also might be good to include a HEX diff breakdown because if the
    # diff is just ordering then the textproto conversion might correct the
    # error and make it look like there is no diff but the test still fails.

    return False


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
# get_waypoint_textproto
#
# Reads a waypoint protobin and returns a stringy textproto value of the
# contents of the protobin. This makes it easier to diff the contents but also
# can be used to easily inspect the values of the protobin.
################################################################################
def get_waypoint_textproto(protobin_path: str) -> str:
    proto_schema_path = "../proto/waypoint.proto"
    proto_schema_basedir = "../proto"

    if not os.path.exists(protobin_path):
        return ""

    with open(protobin_path, 'rb') as f:
        result = subprocess.run(
            [
                "protoc",
                "--decode=waypoint.Waypoint",
                "--proto_path=" + proto_schema_basedir,
                proto_schema_path
            ],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            stdin=f,
        )

        # TODO: sanity check return code, stdout, and stderr

        return result.stdout.decode("utf-8")
