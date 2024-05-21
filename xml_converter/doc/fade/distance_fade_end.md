---
name: Distance Fade End 
type: Float32
applies_to: [Icon, Trail]
xml_fields: [FadeFar, DistanceFadeEnd]
protobuf_field: distance_fade_end
examples:
  - "850"
  - "600"
  - "1100"
---
Any part of the object that is farther then this value will be at 0 alpha. 
Defaults to 900

Notes
=====
https://blishhud.com/docs/markers/attributes/fade
