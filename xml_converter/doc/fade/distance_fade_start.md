---
name: Distance Fade Start
type: Float32
applies_to: [Icon, Trail]
xml_fields: [FadeNear, DistanceFadeStart]
protobuf_field: distance_fade_start
examples:
  - "650"
  - "400"
  - "900"
---
Any part of the object that is farther then this value will begin to alpha fade gradually until it reaches 0 alpha at Distance Fade End.
Defaults to 700

Notes
=====
https://blishhud.com/docs/markers/attributes/fade
