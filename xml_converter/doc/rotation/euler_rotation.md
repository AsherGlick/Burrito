---
name: Euler Rotation
type: CompoundValue
applies_to: ["Icon"]
xml_fields: ["Rotate"]
compatability: [TacO, Burrito, BlishHUD]
protobuf_field: euler_rotation
xml_parent_export: ['X Rotation', 'Y Rotation', 'Z Rotation']
xml_child_export: []
components:
- name: X Rotation
  type: Float32
  xml_fields: [RotateX]
  protobuf_field: "x"
  compatability: [TacO, Burrito, BlishHUD]
  

- name: Y Rotation
  type: Float32
  xml_fields: [RotateY]
  protobuf_field: "y"
  compatability: [TacO, Burrito, BlishHUD]
  

- name: Z Rotation
  type: Float32
  xml_fields: [RotateZ]
  protobuf_field: "z"
  compatability: [TacO, Burrito, BlishHUD]
  
---
Euler X Y Z rotation.

Notes
=====
https://blishhud.com/docs/markers/attributes/rotate
