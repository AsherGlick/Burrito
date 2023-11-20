---
name: Color
type: CompoundCustomClass
class: Color
applies_to: [Icon, Trail]
xml_fields: [Color, BHColor]
protobuf_field: rgba_color
xml_bundled_components: ['Red', 'Green', 'Blue']
xml_separate_components: ['Alpha']
components:
- name: Red
  type: Float32
  xml_fields: [Red]
  protobuf_field: rgba_color

- name: Green
  type: Float32
  xml_fields: [Green]
  protobuf_field: rgba_color
    
- name: Blue
  type: Float32
  xml_fields: [Blue]
  protobuf_field: rgba_color

- name: Alpha
  type: Float32
  xml_fields: [Alpha]
  protobuf_field: rgba_color


---
A multiplier color to tint the raw albedo texture of a marker of trail texture. (Unclear) Solid white will result in no change, solid black will result in a black texture.

Notes
=====
https://blishhud.com/docs/markers/attributes/texture
https://blishhud.com/docs/markers/attributes/iconfile
https://blishhud.com/docs/markers/attributes/color
https://blishhud.com/docs/markers/attributes/animspeed

