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
  examples: ["180", "90", "0", "85", "-90", "0.01"]

- name: Y Rotation
  type: Float32
  xml_fields: [RotateY]
  protobuf_field: "y"
  examples: ["10", "-18", "0", "-1.5", "20", "359"]

- name: Z Rotation
  type: Float32
  xml_fields: [RotateZ]
  protobuf_field: "z"
  examples: ["-38", "-10.55", "-0.65", "123", "53.5", "-75"]

---
Euler X Y Z rotation in degrees.
Can be any floating point value but will be modulo 360 when applied

Notes
=====
https://blishhud.com/docs/markers/attributes/rotate
