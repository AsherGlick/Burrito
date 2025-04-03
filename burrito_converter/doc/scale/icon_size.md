---
name: Icon Size
type: Float32
applies_to: [Icon]
xml_fields: [IconSize]
#151 This field should be set to 0 in the protobuf when equal to the default value
exclude_from_protobuf: True
protobuf_field: tentative__scale
---
Multiplier on the size of an image (i.e. 1 is 100%, 2 is 200%).

Notes
=====
https://blishhud.com/docs/markers/attributes/iconfile
