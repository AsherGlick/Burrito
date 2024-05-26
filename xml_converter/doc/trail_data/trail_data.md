---
name: Trail Data
type: Custom
class: TrailData
xml_fields: ["TrailData"]
uses_file_path: true
protobuf_field: trail_data
applies_to: [Trail]
examples:
  - "trails/my_trail.trl"
  - "jumping_puzzle_path.trl"
custom_functions:
  read.xml:
    function: xml_attribute_to_trail_data
    side_effects: [Map ID]
  write.xml:
    function: trail_data_to_xml_attribute
    side_effects: [Map ID]
---

The coordinates of each point along a trail path.

In the TacO format this attribute is a string that points to a file which contains this information packed in binary. The binary format consists of

- 4 bytes for an integer that represents the `.trl` format version number
- 4 bytes for an integer that represents the Map ID which this trail is for
- Any number of 12 byte sets, containing 3 32 bit floats

Example `.trl` parser
--------------------------------------------------------------------------------

```python
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
```
