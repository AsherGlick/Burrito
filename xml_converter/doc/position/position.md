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

- name: Y Position
  type: Float32
  xml_fields: [YPos, PositionY]
  protobuf_field: "y"

- name: Z Position
  type: Float32
  xml_fields: [ZPos, PositionZ]
  protobuf_field: "z"
---
An XYZ location of a point in the game world.

Notes
=====
https://blishhud.com/docs/markers/attributes/position
https://blishhud.com/docs/markers/attributes/heightoffset
