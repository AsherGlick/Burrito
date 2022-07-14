---
attribute_category: Texture
fields:
  - name: Texture
    description: The path to an image which contains the texture that will be present on a trail.
    type: Custom
    class: Image
    applies_to: [Trail]
    xml_fields: [Texture]
    protobuf_field: texture
    compatability: [TacO, BlishHUD, Burrito]

  - name: Icon
    description: The path to an image which contains the texture that will be present on an icon.
    type: Custom
    class: Image
    applies_to: [Icon]
    xml_fields: [IconFile]
    protobuf_field: texture
    compatability: [TacO, BlishHUD, Burrito]

  - name: Color
    description: A multiplier color to tint the raw albedo texture of a marker of trail texture. (Unclear) Solid white will result in no change, solid black will result in a black texture.
    type: Custom
    class: Color
    applies_to: [Icon, Trail]
    xml_fields: [Color]
    protobuf_field: color
    compatability: [TacO, BlishHUD, Burrito]

  - name: Animation Speed
    description: The speed which the texture should be moving across the object.
    type: Float32
    applies_to: [Trail]
    xml_fields: ["AnimSpeed", "AnimationSpeed"]
    protobuf_field: animation_speed
    compatability: [TacO, BlishHUD, Burrito]
---

Notes
=====
https://blishhud.com/docs/markers/attributes/texture
https://blishhud.com/docs/markers/attributes/iconfile
https://blishhud.com/docs/markers/attributes/color
https://blishhud.com/docs/markers/attributes/animspeed
