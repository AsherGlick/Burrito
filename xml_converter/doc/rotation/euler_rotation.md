---
name: Euler Rotation
type: CompoundValue
applies_to: ["Icon"]
xml_fields: ["Rotate"]
protobuf_field: euler_rotation
xml_bundled_components: ['X Rotation', 'Y Rotation', 'Z Rotation']
xml_separate_components: []
components:
- name: X Rotation
  type: Float32
  xml_fields: [RotateX]
  protobuf_field: "x"

- name: Y Rotation
  type: Float32
  xml_fields: [RotateY]
  protobuf_field: "y"

- name: Z Rotation
  type: Float32
  xml_fields: [RotateZ]
  protobuf_field: "z"

---
Euler X Y Z rotation.

Notes
=====
https://blishhud.com/docs/markers/attributes/rotate
