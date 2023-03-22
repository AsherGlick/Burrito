---
name: Color
type: CompoundCustomClass
class: Color
applies_to: [Icon, Trail]
xml_fields: [Color, BHColor]
protobuf_field: rgba_color
compatability: [TacO, BlishHUD, Burrito]
xml_parent_export: "{Red},{Green},{Blue}"
xml_child_export: ["Alpha"]

components:
    
    - name: Red # fake, to make thing "square"
      type: Float32
      xml_fields: [Red]
      protobuf_field: rgba_color
      compatability: [TacO, Burrito, BlishHUD]

    - name: Green
      type: Float32
      xml_fields: [Green]
      protobuf_field: rgba_color
      compatability: [TacO, Burrito, BlishHUD]
    
    - name: Blue
      type: Float32
      xml_fields: [Blue]
      protobuf_field: rgba_color
      compatability: [TacO, Burrito, BlishHUD]

    - name: Alpha
      type: Float32
      xml_fields: [Alpha]
      protobuf_field: rgba_color
      compatability: [TacO, Burrito, BlishHUD]


---
A multiplier color to tint the raw albedo texture of a marker of trail texture. (Unclear) Solid white will result in no change, solid black will result in a black texture.

Notes
=====
https://blishhud.com/docs/markers/attributes/texture
https://blishhud.com/docs/markers/attributes/iconfile
https://blishhud.com/docs/markers/attributes/color
https://blishhud.com/docs/markers/attributes/animspeed

