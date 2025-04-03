---
name: Position
type: CompoundValue
applies_to: ["Icon"]
xml_fields: ["Position"]
protobuf_field: position
xml_bundled_components: []
xml_separate_components: ["X Position", "Y Position", "Z Position"]
components:
- name: X Position
  type: Float32
  xml_fields: [XPos, PositionX]
  protobuf_field: "x"
  examples: ["166.735", "1110.4", "-433.615", "-196.412"]

- name: Y Position
  type: Float32
  xml_fields: [YPos, PositionY]
  protobuf_field: "y"
  examples: ["81.727", "24.5189", "-9.12564", "3.91473"]

- name: Z Position
  type: Float32
  xml_fields: [ZPos, PositionZ]
  protobuf_field: "z"
  examples: ["-103.464", "-135.401", "164.66", "625.115"]

---
An XYZ location of a point in the game world.

Notes
=====
https://blishhud.com/docs/markers/attributes/position
https://blishhud.com/docs/markers/attributes/heightoffset
