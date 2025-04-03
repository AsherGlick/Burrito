---
name: Trail Scale
type: Float32
applies_to: [Trail]
xml_fields: [TrailScale]
#151 This field should be set to 0 in the protobuf when equal to the default value
exclude_from_protobuf: True
protobuf_field: scale
---
Adjusts the width of a trail mesh as a multiplier to the default width.

Notes
=====
https://blishhud.com/docs/markers/attributes/trailscale/
