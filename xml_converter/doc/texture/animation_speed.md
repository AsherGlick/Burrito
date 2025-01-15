---
name: Animation Speed
type: Float32
applies_to: [Trail]
xml_fields: ["AnimSpeed", "AnimationSpeed"]
#151 This field should be set to 0 in the protobuf when equal to the default value
exclude_from_protobuf: True
protobuf_field: animation_speed
---
The speed which the texture should be moving across the object.

Notes
=====
https://blishhud.com/docs/markers/attributes/texture
https://blishhud.com/docs/markers/attributes/iconfile
https://blishhud.com/docs/markers/attributes/color
https://blishhud.com/docs/markers/attributes/animspeed
