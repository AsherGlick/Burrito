from .proto_utils import compare_binary_file
from typing import List
import difflib
import os
import struct


def compare_trails(
    expected_trail_path: str,
    actual_trail_path: str
) -> List[str]:
    files_are_equal = compare_binary_file(expected_trail_path, actual_trail_path)

    if files_are_equal:
        return []

    expected_textproto = get_texttrail(expected_trail_path)
    actual_textproto = get_texttrail(actual_trail_path)

    diff = list(difflib.unified_diff(actual_textproto.split("\n"), expected_textproto.split("\n"), fromfile=actual_trail_path, tofile=expected_trail_path, lineterm=""))

    if len(diff) == 0:
        diff = ["Something went wrong diffing {} and {}.".format(expected_trail_path, actual_trail_path)]

    return list(diff)


def get_texttrail(trail_path: str) -> str:
    if not os.path.exists(trail_path):
        return ""

    lines = []
    with open(trail_path, 'rb') as f:
        version: int = struct.unpack("<i", f.read(4))[0]
        lines.append("Version: " + str(version))
        map_id: int = struct.unpack("<i", f.read(4))[0]
        lines.append("MapID: " + str(map_id))

        point_index = 0
        while point_bytes := f.read(12):
            x, y, z = struct.unpack("<fff", point_bytes)
            # points.append((x, y, z))
            lines.append("Point {}: {} {} {}".format(point_index, x, y, z))
            point_index += 1

    return "\n".join(lines)
