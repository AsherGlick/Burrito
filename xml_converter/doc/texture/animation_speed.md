---
name: Animation Speed
type: Float32
applies_to: [Trail]
xml_fields: ["AnimSpeed", "AnimationSpeed"]
protobuf_field: animation_speed
custom_functions:
  read.xml:
    function: default_value_one_xml_attribute_to_float
    side_effects: []
---
The speed which the texture should be moving across the object.

Notes
=====
https://blishhud.com/docs/markers/attributes/texture
https://blishhud.com/docs/markers/attributes/iconfile
https://blishhud.com/docs/markers/attributes/color
https://blishhud.com/docs/markers/attributes/animspeed
