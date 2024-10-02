from typing import Tuple, List
import struct

with open("./demo.trl", 'rb') as f:
    version: int = struct.unpack("<i", f.read(4))[0]
    map_id: int = struct.unpack("<i", f.read(4))[0]

    points: List[Tuple[float, float, float]] = []
    while point_bytes := f.read(12):
        x, y, z = struct.unpack("<fff", point_bytes)
        points.append((x, y, z))

print("Version Number:", version)
print("Map ID:", map_id)
print("Points:", points)
