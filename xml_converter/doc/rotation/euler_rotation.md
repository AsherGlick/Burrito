---
name: Euler Rotation
type: CompoundValue
applies_to: ["Icon"]
xml_fields: ["Rotation"]
compatability: [TacO, Burrito, BlishHUD]
protobuf_field: euler_rotation
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
  
xml_export: "Parent Only"
---
Euler X Y Z rotation.

Notes
=====
https://blishhud.com/docs/markers/attributes/rotate
