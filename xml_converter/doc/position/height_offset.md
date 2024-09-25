---
name: Height Offset
type: Float32
applies_to: ["Icon"]
xml_fields: ["HeightOffset", "BHHeightOffset"]
#151 This field should be set to 0 in the protobuf when equal to the default value
protobuf_field: null #height_offset
examples:
  - "0"
---
A vertical offset of the object from the recorded position.

Notes
=====
https://blishhud.com/docs/markers/attributes/heightoffset
